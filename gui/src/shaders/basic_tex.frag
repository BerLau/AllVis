#version 330 core
#define LIGHT_MAX 4

out vec4 frag_color;

in vec3 frag_position;
in vec3 frag_normal;
in vec2 texcoord;
in mat3 TBN;
in vec3 view_pos;

#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2
struct Light
{
    vec3 position;
    vec3 direction;
    vec3 color;
    int type;
    float intensity;
};

struct Material
{
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;

    sampler2D albedo_map;
    sampler2D metallic_map;
    sampler2D roughness_map;
    sampler2D ao_map;
    sampler2D normal_map;
};

uniform int u_light_num;
uniform Light[LIGHT_MAX] u_lights;
uniform Material u_material;
uniform mat4 u_view;

float near = 0.1;
float far = 100.0;

void main()
{
    vec3 view_dir = normalize(-frag_position);
    vec3 normal = normalize(frag_normal);
    vec3 albedo = u_material.albedo;
    float metallic = u_material.metallic;
    float roughness = u_material.roughness;
    float ao = u_material.ao;

    if(u_material.albedo_map != 0)
    {
        albedo = texture(u_material.albedo_map, texcoord).rgb;
    }
    if(u_material.metallic_map != 0)
    {
        metallic = texture(u_material.metallic_map, texcoord).r;
    }
    if(u_material.roughness_map != 0)
    {
        roughness = texture(u_material.roughness_map, texcoord).r;
    }
    if(u_material.ao_map != 0)
    {
        ao = texture(u_material.ao_map, texcoord).r;
    }

    vec3 result = vec3(0.0);
    for(int i =0; i< u_light_num; ++i){
        // treat all light as point light
        vec3 light_pos = vec3(u_view*vec4(u_lights[i].position, 1.0));
        vec3 light_dir = normalize(light_pos - frag_position);
        vec3 light_color = u_lights[i].color;
        float light_intensity = u_lights[i].intensity;
        float light_type = u_lights[i].type;

        float distance = length(light_pos - frag_position);
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

        vec3 ambient = light_intensity * light_color * albedo * ao;

        vec3 diffuse = vec3(0.0);
        vec3 specular = vec3(0.0);

        vec3 halfway_dir = normalize(light_dir + view_dir);
        float NdotL = max(dot(normal, light_dir), 0.0);
        float NdotH = max(dot(normal, halfway_dir), 0.0);
        float VdotH = max(dot(view_dir, halfway_dir), 0.0);
        diffuse = light_intensity * light_color * albedo * NdotL * attenuation;
        specular = light_intensity * light_color * pow(NdotH, roughness) * attenuation;
        
        result += (ambient + diffuse + specular);
    }
    
    frag_color = vec4(result, 1.0);
}