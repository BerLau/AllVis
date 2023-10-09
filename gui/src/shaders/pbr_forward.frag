#version 330 core
#define MAX_LIGHTS 100
#define MAX_DIR_LIGHTS 4
#define PI 3.14159265359

layout (location = 0) out vec4 frag_color;
layout (location = 1) out vec4 bright_color;

in OUT{
    mat3 tbn;
    vec3 position;
    vec2 texcoord;
    vec4 pos_in_light_space;
} in_data;

struct Material {
    bool has_albedo_map;
    bool has_normal_map;
    bool has_roughness_map;
    bool has_ao_map;
    bool has_height_map;

    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
    
    sampler2D albedo_map;
    sampler2D normal_map;
    sampler2D roughness_map;
    sampler2D ao_map;
    sampler2D height_map;
};

struct Light
{
    vec3 position;
    vec3 direction;
    vec3 color;

    float cutOff;
    float outerCutOff;
    float intensity;
    float radius;
};

uniform Material material;
uniform Light point_lights[MAX_LIGHTS];
uniform Light directional_lights[MAX_DIR_LIGHTS];
uniform mat4 u_view;

// uniform samplerCube skybox;
uniform samplerCube irradiance_map;
uniform samplerCube prefilter_map;
uniform sampler2D brdf_lut;

uniform sampler2D shadow_map;
uniform float u_far_plane;

uniform float bias=0.005;


uniform int point_light_size;
uniform int directional_light_size;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);

float directional_shadow_calculation(vec4 frag_position, vec3 normal, vec3 lightDir);


void main(){
    bright_color = vec4(0.0, 0.0, 0.0, 1.0);
    vec3 albedo = material.albedo;
    if(material.has_albedo_map){
        albedo = texture(material.albedo_map, in_data.texcoord).rgb;
    }

    vec3 normal = vec3(0.0,0.0,1.0);
    if(material.has_normal_map){
        vec3 normal_tbn = normalize(texture(material.normal_map, in_data.texcoord).rgb*2.0-1.0);
        normal  = normalize((u_view * vec4(in_data.tbn*normal_tbn, 0.0)).xyz);
    }
    else{
        normal = normalize((u_view * vec4(in_data.tbn*normal,0.0)).xyz);
    }

    float metallic = material.metallic;

    float roughness = material.roughness;
    if(material.has_roughness_map){
        roughness = texture(material.roughness_map, in_data.texcoord).r;
    }

    float ao = material.ao;
    if(material.has_ao_map){
        ao = texture(material.ao_map, in_data.texcoord).r;
    }

    vec3 frag_pos = in_data.position;
    vec3 view_dir = normalize(-frag_pos);

    vec3 reflect_dir = reflect(-view_dir, normal);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
    vec3 Lo = vec3(0.0);

    for(int i=0; i< point_light_size; i++){
        vec3 light_pos = (u_view*vec4(point_lights[i].position,1.0)).xyz;
        vec3 L = normalize(light_pos - frag_pos);
        vec3 H = normalize(view_dir + L);
        float distance = length(light_pos - frag_pos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = point_lights[i].color * attenuation;

        float NDF = DistributionGGX(normal, H, roughness);
        float G = GeometrySmith(normal, view_dir, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, view_dir), 0.0), F0);

        vec3 nominator = NDF * G * F;
        float denominator = 4.0 * max(dot(normal, view_dir), 0.0) * max(dot(normal, L), 0.0);
        vec3 specular = nominator / max(denominator, 0.001);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        float NdotL = max(dot(normal, L), 0.0);
        
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    for(int i=0; i< directional_light_size; i++){

        vec3 L = normalize(-(u_view * vec4(directional_lights[i].direction,0.0)).xyz);
        vec3 H = normalize(view_dir + L);
        vec3 radiance = directional_lights[i].color;

        float NDF = DistributionGGX(normal, H, roughness);
        float G = GeometrySmith(normal, view_dir, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, view_dir), 0.0), F0);

        vec3 nominator = NDF * G * F;
        float denominator = 4.0 * max(dot(normal, view_dir), 0.0) * max(dot(normal, L), 0.0);
        vec3 specular = nominator / max(denominator, 0.001);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        float NdotL = max(dot(normal, L), 0.0);
        
        vec3 r= (kD * albedo / PI + specular) * radiance * NdotL;

        float shadow = directional_shadow_calculation(in_data.pos_in_light_space, normal, L);
        Lo += (1-shadow)*r;

    }

    vec3 F = fresnelSchlickRoughness(max(dot(normal, view_dir), 0.0), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    vec3 irradiance = texture(irradiance_map, normal).rgb;
    vec3 diffuse = irradiance * albedo;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefiltered_color = textureLod(prefilter_map, reflect_dir, roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf  = texture(brdf_lut, vec2(max(dot(normal, view_dir), 0.0), roughness)).rg;
    vec3 specular = prefiltered_color * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;

    vec3 color = ambient + Lo;

    frag_color = vec4(color, 1.0);    
    
}

float DistributionGGX(vec3 N, vec3 H, float roughness){
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness){
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness){
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0){
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness){
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

float directional_shadow_calculation(vec4 frag_position, vec3 normal, vec3 lightDir){
    // perform perspective divide
    vec3 proj_coords = frag_position.xyz / frag_position.w;
    // transform to [0,1] range
    proj_coords = proj_coords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closest_depth = texture(shadow_map, proj_coords.xy).r; 
    // get depth of current fragment from light's perspective
    float current_depth = proj_coords.z;
    
    float bias_f = max(0.05 * (1.0 - dot(normal, lightDir)), bias);

    // check whether current frag pos is in shadow
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadow_map, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadow_map, proj_coords.xy + vec2(x, y) * texelSize).r; 
            shadow += current_depth - bias_f > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(current_depth > 1.0)
        shadow = 0.0;
        
    return shadow;
}

