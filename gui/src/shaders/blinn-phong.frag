/*
fragment shader for Blinn-Phong shading
in: mat3 tbn, vec3 frag_position, vec2 texcoord
out: vec4 frag_color
uniform: mat4 view, Material u_material, Light u_lights[MAX_LIGHTS], int
u_light_num
*/
#version 420 core
#define MAX_LIGHTS 16
#define LIGHT_TYPE_PARALLEL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

layout(location = 0) out vec4 frag_color;

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
  vec3 color;
  float metallic;
  float roughness;
  float ao;

  sampler2D albedo_map;
  sampler2D metallic_map;
  sampler2D roughness_map;
  sampler2D ao_map;
  sampler2D normal_map;
  sampler2D height_map;

  bool has_albedo_map;
  bool has_metallic_map;
  bool has_roughness_map;
  bool has_ao_map;
  bool has_normal_map;
  bool is_emissive;
  bool has_height_map;
};

uniform mat4 u_view;
uniform Material u_material;
uniform Light u_lights[MAX_LIGHTS];
uniform int u_light_num;

vec3 get_ambient() {
  if (u_material.has_albedo_map) {
    return u_material.color * texture(u_material.albedo_map, texcoord).rgb;
  }
  return u_material.color;
}

vec3 get_diffuse(vec3 normal, vec3 light_dir, vec3 light_color) {
  float diff = max(dot(normal, light_dir), 0.0);
  return diff * light_color;
}

vec3 get_specular(vec3 normal, vec3 light_dir, vec3 view_dir, float roughness,
                  vec3 light_color) {
  vec3 halfway_dir = normalize(light_dir + view_dir);
  float spec = pow(max(dot(normal, halfway_dir), 0.0), 1.0 / roughness);
  return spec * light_color;
}

vec3 get_emissive() {
  if (u_material.is_emissive) {
    if (u_material.has_albedo_map) {
      return texture(u_material.albedo_map, texcoord).rgb;
    }
    return u_material.color;
  }
  return vec3(0.0);
}

vec3 get_normal() {
  if (u_material.has_normal_map) {
    return normalize(
        tbn * (2.0 * texture(u_material.normal_map, texcoord).rgb - 1.0));
  }
  return normalize(tbn * vec3(0.0, 0.0, 1.0));
}

void main() {
  vec3 normal = get_normal();
  vec3 view_dir = normalize(-frag_position);
  vec3 ambient = get_ambient();
  vec3 diffuse = vec3(0.0);
  vec3 specular = vec3(0.0);
  vec3 emissive = get_emissive();

  for (int i = 0; i < u_light_num; i++) {
    vec3 light_dir = normalize(u_lights[i].position - frag_position);
    vec3 light_color = u_lights[i].color * u_lights[i].intensity;
    if (u_lights[i].type == LIGHT_TYPE_PARALLEL) {
      light_dir = normalize(-u_lights[i].direction);
    } else {
      vec3 light_pos = vec3(u_view * vec4(u_lights[i].position, 1.0));
      light_dir = normalize(light_pos - frag_position);
      float distance = length(light_pos - frag_position);
      float attenuation = 1.0 / (distance * distance);
      light_color *= attenuation;
      if (u_lights[i].type == LIGHT_TYPE_SPOT) {
        float theta = dot(light_dir, normalize(-u_lights[i].direction));
        float epsilon = u_lights[i].direction.z;
        float intensity = clamp((theta - epsilon) / (1.0 - epsilon), 0.0, 1.0);
        light_color *= intensity;
      }
    }
    
    diffuse += get_diffuse(normal, light_dir, light_color);
    specular += get_specular(normal, light_dir, view_dir,
                             u_material.roughness, light_color);
  }
  frag_color = vec4(ambient + diffuse + specular + emissive, 1.0);
}
