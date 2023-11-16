/*
fragment shader for pbr shading
in: mat3 tbn, vec3 frag_position, vec2 texcoord
out: vec4 fragColor, vec4 brightColor
uniform: Material u_material, Light u_lights[MAX_LIGHTS], int u_light_num,
mat4 u_view, bool u_ibl_enable, vec3 u_env_color, samplerCube
u_irradiance_map, samplerCube u_prefilter_map, sampler2D u_brdf_lut
*/
#version 420 core
#define PI 3.14159265359
#define MAX_LIGHTS 33
#define LIGHT_TYPE_PARALLEL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

layout(location = 0) out vec4 frag_color;
layout(location = 1) out vec4 bright_color;

in mat3 tbn;
in vec3 frag_position;
in vec2 texcoord;

struct Light {
  vec3 position;
  vec3 direction;
  vec3 color;
  int type;
  float intensity;
};

struct Material {
  vec3 albedo;
  vec3 emissive;
  float metallic;
  float roughness;
  float ao;
  float height_scale;

  sampler2D albedo_map;
  sampler2D metallic_map;
  sampler2D roughness_map;
  sampler2D ao_map;
  sampler2D normal_map;
  sampler2D height_map;
  sampler2D emissive_map;

  float emissive_intensity;
  float emissive_texture_factor;
  float normal_texture_factor;
  float height_texture_factor;
  float albedo_texture_factor;
  float metallic_texture_factor;
  float roughness_texture_factor;
  float ao_texture_factor;
};

uniform Material u_material;
uniform Light u_lights[MAX_LIGHTS];
uniform int u_light_num;
uniform mat4 u_view;

uniform vec3 u_env_color;
uniform samplerCube u_irradiance_map;
uniform samplerCube u_prefilter_map;
uniform sampler2D u_brdf_lut;
uniform float u_ibl_enable;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);

const float MAX_REFLECTION_LOD = 4.0;

vec3 to_srgb(vec3 color) { return pow(color, vec3(2.2)); }

void main() {
  vec3 tex_normal = texture(u_material.normal_map, texcoord).rgb;
  tex_normal = normalize(tex_normal * 2.0 - 1.0);
  vec3 normal =
      mix(vec3(0.0, 0.0, 1.0), tex_normal, u_material.normal_texture_factor);
  // view spaced normal
  normal = normalize((u_view * vec4(tbn * normal, 0.0)).xyz);
  // view spaced view direction
  vec3 view_dir = normalize(-frag_position);

  vec3 tex_albedo = texture(u_material.albedo_map, texcoord).rgb;
  vec3 albedo =
      mix(u_material.albedo, tex_albedo, u_material.albedo_texture_factor);

  float tex_metallic = texture(u_material.metallic_map, texcoord).r;
  float metallic = mix(u_material.metallic, tex_metallic,
                       u_material.metallic_texture_factor);

  float tex_roughness = texture(u_material.roughness_map, texcoord).r;
  float roughness = mix(u_material.roughness, tex_roughness,
                        u_material.roughness_texture_factor);

  float tex_ao = texture(u_material.ao_map, texcoord).r;
  float ao = mix(u_material.ao, tex_ao, u_material.ao_texture_factor);

  vec3 tex_emissive = texture(u_material.emissive_map, texcoord).rgb;
  vec3 emissive =
      u_material.emissive_intensity * mix(u_material.emissive, tex_emissive,
                                          u_material.emissive_texture_factor);

  vec3 F0 = vec3(0.04);
  F0 = mix(F0, albedo, metallic);
  vec3 Lo = vec3(0.0);

  for (int i = 0; i < u_light_num; ++i) {
    vec3 radiance;
    vec3 L;
    vec3 H;
    Light light = u_lights[i];

    if (light.type == LIGHT_TYPE_PARALLEL) {
      L = normalize((u_view * vec4(light.direction, 0.0)).xyz);
      H = normalize(view_dir + L);
      radiance = light.color;
    } else {
      vec3 light_pos = (u_view * vec4(light.position, 1.0)).xyz;
      L = normalize(light_pos - frag_position);
      H = normalize(view_dir + L);
      float distance = length(light_pos - frag_position);
      float attenuation = 1.0 / (distance * distance);
      radiance = light.color * attenuation;
      if (light.type == LIGHT_TYPE_SPOT) {
        float spot = dot(normalize(light.direction), -L);
        if (spot > light.intensity) {
          radiance *= pow(spot, light.intensity);
        } else {
          radiance = vec3(0.0);
        }
      }
    }

    float NDF = DistributionGGX(normal, H, roughness);
    float G = GeometrySmith(normal, view_dir, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, view_dir), 0.0), F0);

    vec3 nominator = NDF * G * F;
    float denominator =
        4.0 * max(dot(normal, view_dir), 0.0) * max(dot(normal, L), 0.0);
    vec3 specular = nominator / max(denominator, 0.001);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    float NdotL = max(dot(normal, L), 0.0);

    Lo += (kD * albedo / PI + specular) * radiance * NdotL;
  }

  vec3 F =
      fresnelSchlickRoughness(max(dot(normal, view_dir), 0.0), F0, roughness);

  vec3 kS = F;
  vec3 kD = 1.0 - kS;
  kD *= 1.0 - metallic;

  // compute Nomal, ViewDir in the world space
  vec3 N = normalize((u_view * vec4(normal, 0.0)).xyz);
  vec3 V = normalize((u_view * vec4(view_dir, 0.0)).xyz);
  vec3 R = reflect(-V, N);

  vec3 irradiance = texture(u_irradiance_map, N).rgb;
  vec3 diffuse = kD * albedo * irradiance;

  vec3 prefiltered_color =
      textureLod(u_prefilter_map, R, roughness * MAX_REFLECTION_LOD).rgb;
  vec2 brdf = texture(u_brdf_lut, vec2(max(dot(N, V), 0.0), roughness)).rg;

  vec3 specular = kS * prefiltered_color * (F * brdf.x + brdf.y);

  vec3 ambient = (diffuse + specular) * ao;
  vec3 color = Lo + ambient + emissive;

  frag_color = vec4(color, 1.0);

  float brightness = dot(frag_color.rgb, vec3(0.2126, 0.7152, 0.0722));
  bright_color = vec4(brightness, 0.0, 0.0, 1.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
  float a = roughness * roughness;
  float a2 = a * a;
  float NdotH = max(dot(N, H), 0.0);
  float NdotH2 = NdotH * NdotH;

  float nom = a2;
  float denom = (NdotH2 * (a2 - 1.0) + 1.0);
  denom = PI * denom * denom;

  return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
  float r = (roughness + 1.0);
  float k = (r * r) / 8.0;

  float nom = NdotV;
  float denom = NdotV * (1.0 - k) + k;

  return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
  float NdotV = max(dot(N, V), 0.0);
  float NdotL = max(dot(N, L), 0.0);
  float ggx2 = GeometrySchlickGGX(NdotV, roughness);
  float ggx1 = GeometrySchlickGGX(NdotL, roughness);

  return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
  return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
  return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}
