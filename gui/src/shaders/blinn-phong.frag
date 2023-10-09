#version 330 core
layout (location = 0) out vec4 frag_color;
layout (location = 1) out vec4 bright_color;

#define MAX_LIGHTS 100
#define MAX_DIR_LIGHTS 4

struct Light
{
    vec3 position;
    vec3 direction;
    vec3 color;
    // float constant;
    // float linear;
    // float quadratic;
    float cutOff;
    float outerCutOff;
    float intensity;
    float radius;
};


struct Material {
    bool has_albedo_map;
    bool has_normal_map;
    bool has_metallic_map;
    bool has_roughness_map;
    bool has_ao_map;
    bool has_height_map;

    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
    
    sampler2D albedo_map;
    sampler2D normal_map;
    sampler2D metallic_map;
    sampler2D roughness_map;
    sampler2D ao_map;
    sampler2D height_map;
};


in OUT{
    vec3 position;
    vec2 texcoord;
    vec4 pos_in_light_space;
    vec3 tspace_view_pos;
    vec3 tspace_frag_pos;
    mat3 tbn;
} f_in;


uniform Light directional_lights[MAX_DIR_LIGHTS];
uniform Light point_lights[MAX_LIGHTS];

uniform int directional_light_size;
uniform int point_light_size;

uniform Material material;
uniform sampler2D shadow_map;

uniform float u_far_plane;

vec3 calc_dir_light(Light light, vec3 normal, vec3 view_dir, vec3 diffuse_color, vec3 specular_color);
vec3 calc_point_light(Light light, vec3 normal, vec3 view_dir, vec3 diffuse_color, vec3 specular_color, float fl_distance);
vec3 calc_spot_light(Light light, vec3 normal, vec3 view_dir, vec3 diffuse_color, vec3 specular_color, float fl_distance);

float directional_shadow_calculation(vec4 frag_pos_lightspace, vec3 normal, vec3 lightDir);
float point_shadow_calculation(vec3 frag_pos, vec3 light_position);

vec2 parallax_mapping(sampler2D height_map, float height_scale, vec2 texcoord, vec3 view_dir);

// float  gamma = 2.2;
float bias = 0.005;

void main()
{
    vec3 view_dir = normalize(f_in.tspace_view_pos - f_in.tspace_frag_pos);

    vec3 diffuse_color = material.albedo;
    vec3 specular_color = material.metallic * vec3(0.5);
    vec2 texcoord = f_in.texcoord;
    vec3 normal = vec3(0.0,0.0,1.0);
    if(material.has_normal_map){
        normal = texture(material.normal_map, texcoord).rgb;
        normal = normalize(normal * 2.0 - 1.0);
    }

    if(material.has_albedo_map){
        diffuse_color = texture(material.albedo_map, texcoord).rgb;
        // diffuse_color = pow(diffuse_color, vec3(gamma));
    }

    vec3 result = vec3(0.0);

    for(int i=0; i< directional_light_size; i++){
        result += calc_dir_light(directional_lights[i], normal, view_dir, diffuse_color, specular_color);
    }

    for(int i=0; i<point_light_size; i++){
        float fl_distance = length(point_lights[i].position - f_in.position);
        result += calc_point_light(point_lights[i], normal, view_dir, diffuse_color, specular_color, fl_distance);
    }

    // add minimum ambient
    vec3 ambient = 0.3 * diffuse_color;
    result += ambient;

    frag_color = vec4(result, 1.0);

    // brightness threshold
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    float threshold = 2.2;
    bright_color = vec4(brightness > threshold ? result : vec3(0.0), 1.0);

}


// Definition of functions

vec3 calc_dir_light(Light light, vec3 normal, vec3 view_dir, vec3 diffuse_color, vec3 specular_color)
{
    vec3 light_dir = normalize(f_in.tbn * (-light.direction));
    // diffuse shading
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diff * light.color * diffuse_color;

    // specular shading
    vec3 halfwayDir = normalize(light_dir + view_dir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.roughness);
    vec3 specular = spec * light.color * specular_color;

    // combine results
    vec3 result = (diffuse + specular) * light.intensity;

    float shadow = directional_shadow_calculation(f_in.pos_in_light_space, normal, light_dir);
    result = (1.0 - shadow) * result;
    return result;
}

vec3 calc_point_light(Light light, vec3 normal, vec3 view_dir, vec3 diffuse_color, vec3 specular_color, float fl_distance)
{
    vec3 light_dir = normalize(f_in.tbn * (light.position - f_in.position));
    // diffuse shading
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diff * light.color * diffuse_color;

    // specular shading
    vec3 halfwayDir = normalize(light_dir + view_dir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.roughness);
    vec3 specular = spec * light.color * specular_color;

    // attenuation
    float attenuation = 1.0 / (fl_distance * fl_distance);   

    // combine results
    vec3 result = (diffuse + specular) * attenuation * light.intensity;
    return result;
}

vec3 calc_spot_light(Light light, vec3 normal, vec3 view_dir, vec3 diffuse_color, vec3 specular_color, float fl_distance)
{
    vec3 light_dir = normalize(f_in.tbn * (-f_in.position));
    // diffuse shading
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diff * light.color * diffuse_color;
    // specular shading
    vec3 halfwayDir = normalize(light_dir + view_dir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.roughness);
    vec3 specular = spec * light.color * specular_color;
    // attenuation
    float attenuation = 1.0 / (fl_distance * fl_distance);    

    // spotlight intensity
    float theta = dot(light_dir, light_dir);
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // combine results
    vec3 result = (diffuse + specular) * attenuation * intensity;
    return result;
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
    
    bias = max(0.05 * (1.0 - dot(normal, lightDir)), bias);

    // check whether current frag pos is in shadow
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadow_map, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadow_map, proj_coords.xy + vec2(x, y) * texelSize).r; 
            shadow += current_depth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(current_depth > 1.0)
        shadow = 0.0;
        
    return shadow;
}

// float point_shadow_calculation(vec3 frag_pos, vec3 light_position){
//     vec3 frag_to_light = frag_pos - light_position;
//     float closest_depth = texture(shadow_cubemap, frag_to_light).r;

//     closest_depth *= u_far_plane; // undo mapping [0;1]

//     float current_depth = length(frag_to_light);

//     float shadow = current_depth - bias > closest_depth ? 1.0 : 0.0;

//     return shadow;

// }

vec2 parallax_mapping(sampler2D height_map, float height_scale, vec2 texcoord, vec3 view_dir)
{
    const float min_layers = 8.0;
    const float max_layers = 32.0;
    float num_layers = mix(max_layers, min_layers, abs(dot(vec3(0.0, 0.0, 1.0), view_dir)));
    float layer_depth = 1.0 / num_layers;
    float current_layer_depth = 0.0;
    vec2 parralax = view_dir.xy / view_dir.z * height_scale;
    vec2 delta_texcoord = parralax / num_layers;
    vec2 current_texcoord = texcoord;
    float current_depth_map_value = texture(height_map, current_texcoord).r;
    while(current_layer_depth < current_depth_map_value)
    {
        current_texcoord -= delta_texcoord;
        current_depth_map_value = texture(height_map, current_texcoord).r;
        current_layer_depth += layer_depth;
    }
    return current_texcoord;
}


