#version 330 core
#define MAX_LIGHTS 64
#define MAX_DIR_LIGHTS 4
#define MAX_POINT_LIGHTS 64
#define MAX_SPOT_LIGHTS 16

#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

struct Light
{
    int type;
    vec3 position;
    vec3 direction;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
};


struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 normal;
    vec3 specular;
    vec3 emission;
    float shininess;
    
    sampler2D emission_texture;
    sampler2D albedo_map;
    sampler2D specular_map;
    sampler2D normal_map;

    int emission_texture_id;
    int albedo_map_id;
    int specular_map_id;
    int normal_map_id;
};

out vec4 FragColor;

in vec2 f_texcoord;
in vec3 f_normal;
in vec3 f_position;
in vec4 f_light_space_pos;

uniform Light u_lights[MAX_LIGHTS];

uniform int u_lights_size;

uniform Material material;
uniform vec3 u_view_pos;
uniform sampler2D shadow_map;
uniform samplerCube shadow_cubemap;

uniform float u_far_plane;

vec3 calc_dir_light(Light light, vec3 normal, vec3 viewDir, vec3 diffuse_color, vec3 specular_color);
vec3 calc_point_light(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuse_color, vec3 specular_color);
vec3 calc_spot_light(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuse_color, vec3 specular_color);
float directional_shadow_calculation(vec4 frag_pos_lightspace, Light light);
float shadow_calculation(vec3 frag_pos, vec3 light_position);
float  gamma = 2.2;
float bias = 0.005;

void main()
{
    vec3 norm = normalize(f_normal);
    vec3 viewDir = normalize(u_view_pos - f_position);
    vec3 diffuse_color = material.diffuse;
    vec3 specular_color = material.specular;
    vec3 emission_color = material.emission;
    
    if(material.albedo_map_id != 0){
        diffuse_color = texture(material.albedo_map, f_texcoord).rgb;
        diffuse_color = pow(diffuse_color, vec3(gamma));
    }

    if(material.specular_map_id != 0){
        specular_color = texture(material.specular_map, f_texcoord).rgb;
    }

    if(material.emission_texture_id != 0){
        emission_color = texture(material.emission_texture, f_texcoord).rgb;
    }

    vec3 result = vec3(0.0);
    int num_dir_lights = 0;
    int num_point_lights = 0;
    int num_spot_lights = 0;
    for(int i=0; i<u_lights_size; i++){
        if(i >= MAX_LIGHTS)
            break;
        if(u_lights[i].type == LIGHT_TYPE_DIRECTIONAL && num_dir_lights < MAX_DIR_LIGHTS){
            result += calc_dir_light(u_lights[i], norm, viewDir, diffuse_color, specular_color);
            num_dir_lights++;
        }
        else if(u_lights[i].type == LIGHT_TYPE_POINT && num_point_lights < MAX_POINT_LIGHTS){
            result += calc_point_light(u_lights[i], norm, f_position, viewDir, diffuse_color, specular_color);
            num_point_lights++;
        }
        else if(u_lights[i].type == LIGHT_TYPE_SPOT && num_spot_lights < MAX_SPOT_LIGHTS){
            result += calc_spot_light(u_lights[i], norm, f_position, viewDir, diffuse_color, specular_color);
            num_spot_lights++;
        }
    }
    // add minimum ambient
    result += 0.02 * diffuse_color;
    // gamma correction
    result = pow(result, vec3(1.0/gamma));
    FragColor = vec4(result, 1.0);



}


// Definition of functions

vec3 calc_dir_light(Light light, vec3 normal, vec3 viewDir, vec3 diffuse_color, vec3 specular_color)
{
    vec3 lightDir = -light.direction;
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.color * diffuse_color;

    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = spec * light.color * specular_color;

    // combine results
    vec3 result = diffuse + specular;

    float shadow = directional_shadow_calculation(f_light_space_pos, light);
    result = (1.0 - shadow) * result;
    return result;
}

vec3 calc_point_light(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuse_color, vec3 specular_color)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 diffuse = diff * light.color * diffuse_color;
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = spec * light.color * specular_color;

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   

    // combine results
    vec3 result = (diffuse + specular) * attenuation;

    // float shadow = shadow_calculation(fragPos, light.position);
    // result = (1.0 - shadow) * result;

    return result;
}

vec3 calc_spot_light(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuse_color, vec3 specular_color)
{
    vec3 lightDir = -light.direction;
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.color * diffuse_color;
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = spec * light.color * specular_color;
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    // spotlight intensity
    float theta = dot(lightDir, normalize(lightDir));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // combine results
    vec3 result = (diffuse + specular) * attenuation * intensity;
    return result;
}

float directional_shadow_calculation(vec4 frag_pos_lightspace, Light light){
    // perform perspective divide
    vec3 proj_coords = frag_pos_lightspace.xyz / frag_pos_lightspace.w;
    // transform to [0,1] range
    proj_coords = proj_coords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closest_depth = texture(shadow_map, proj_coords.xy).r; 
    // get depth of current fragment from light's perspective
    float current_depth = proj_coords.z;
    vec3 light_dir = normalize(-light.direction);
    bias = max(0.05 * (1.0 - dot(normalize(f_normal), light_dir)), bias);

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
    if(proj_coords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

float shadow_calculation(vec3 frag_pos, vec3 light_position){
    vec3 frag_to_light = frag_pos - light_position;
    float closest_depth = texture(shadow_cubemap, frag_to_light).r;

    closest_depth *= u_far_plane; // undo mapping [0;1]

    float current_depth = length(frag_to_light);

    float shadow = current_depth - bias > closest_depth ? 1.0 : 0.0;

    return shadow;

}



