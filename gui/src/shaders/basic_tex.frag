#version 330 core
#define LIGHT_MAX 4

out vec4 frag_color;

in vec3 frag_position;
in vec3 frag_pos_tbn;
in vec3 view_pos_tbn;
in vec2 texcoord;
in mat3 TBN;

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
    vec3 color;
    float metallic;
    float roughness;
    float ao;

    sampler2D albedo_map;
    sampler2D metallic_map;
    sampler2D roughness_map;
    sampler2D ao_map;
    sampler2D normal_map;

    bool has_albedo_map;
    bool has_metallic_map;
    bool has_roughness_map;
    bool has_ao_map;
    bool has_normal_map;
    bool is_emissive;
};

uniform int u_light_num;
uniform Light[LIGHT_MAX] u_lights;
uniform Material u_material;

float near = 0.1;
float far = 100.0;

void main()
{
    vec3 view_dir = normalize(view_pos_tbn - frag_pos_tbn);
    vec3 normal = vec3(0.0, 0.0, 1.0);

    vec3 albedo = u_material.color;
    float metallic = u_material.metallic;
    float roughness = u_material.roughness;
    float ao = u_material.ao;

    if(u_material.has_normal_map)
    {
        normal = normalize(texture(u_material.normal_map, texcoord).rgb * 2.0 - 1.0);
    }
    if(u_material.is_emissive)
    {
        // set the color of the light source
        // frags that the normal towards the view point has higher intensity
        float intensity = max(dot(normal, view_dir), 0.0);
        frag_color = vec4(u_material.color * intensity, 1.0);
        return;
    }


    if(u_material.has_albedo_map)
    {
        albedo = texture(u_material.albedo_map, texcoord).rgb;
    }
    if(u_material.has_metallic_map)
    {
        metallic = texture(u_material.metallic_map, texcoord).r;
    }
    if(u_material.has_roughness_map)
    {
        roughness = texture(u_material.roughness_map, texcoord).r;
    }
    if(u_material.has_ao_map)
    {
        ao = texture(u_material.ao_map, texcoord).r;
    }

    vec3 result = vec3(0.0);
    for(int i =0; i< u_light_num; ++i){
        // treat all light as point light
        vec3 light_pos = u_lights[i].position;
        vec3 light_dir = normalize(TBN*(light_pos - frag_position));
        vec3 light_color = u_lights[i].color;
        float light_intensity = u_lights[i].intensity;
        float light_type = u_lights[i].type;

        float distance = length(light_pos - frag_position);
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

        vec3 ambient = light_intensity * light_color * albedo * ao;

        vec3 diffuse = vec3(0.0);
        vec3 specular = vec3(0.0);

        float NdotL = max(dot(normal, light_dir), 0.0);
        diffuse = light_intensity * light_color * albedo * NdotL * attenuation;


        vec3 halfway_dir = normalize(light_dir + view_dir);
        float NdotH = max(dot(normal, halfway_dir), 0.0);

        specular = metallic * light_intensity * light_color * pow(NdotH, roughness*64) * attenuation;
        
        result += (ambient + diffuse + specular);
    }
    
    frag_color = vec4(result, 1.0);
}