#version 330 core 
layout (location = 0) out vec4 frag_color;
layout (location = 1) out vec4 bright_color;

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
const int LIGHTS_MAX = 100;

in vec2 f_texcoord;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo_spec;
uniform sampler2D ssao;
uniform sampler2D light_source;

uniform sampler2D shadow_map;
uniform sampler2D light_space_map;

uniform Light directional_lights[4];
uniform Light point_lights[LIGHTS_MAX];

uniform int directional_light_size;
uniform int point_light_size;
uniform int spot_light_size;

uniform mat4 u_light_space_mat;
uniform mat4 u_view_mat;

uniform float shininess;
float bias = 0.005;

vec3 calc_dir_light(Light light, vec3 normal, vec3 view_dir, vec4 frag_pos, vec3 albedo_spec, vec3 specular_colo);

vec3 calc_point_light(Light light, vec3 normal, vec3 view_dir, vec3 frag_pos, vec3 diffuse_color, vec3 specular_color);

float calc_dir_shadow(vec3 normal, vec3 light_dir);

void main(){

    vec4 light_source = texture(light_source, f_texcoord);
    vec4 position = texture(g_position, f_texcoord);
    if(light_source.w > position.w){
        vec3 out_color = pow(light_source.xyz, vec3(2.2));
        frag_color = vec4(out_color, 1.0);
        bright_color = vec4(out_color*0.75, 1.0);
        return;
    }
    vec4 n = texture(g_normal, f_texcoord);
    float ao_factor = texture(ssao, f_texcoord).r;
    vec3 normal = normalize(n.xyz);
    vec3 f_position = position.xyz;
    vec3 view_dir = normalize( - f_position);
    vec4 albedo_spec = texture(g_albedo_spec, f_texcoord);
    vec3 diffuse_color = albedo_spec.rgb;
    vec3 ambient = diffuse_color * (ao_factor*0.3);
    vec3 specular_color = vec3(albedo_spec.a);


    vec3 result = vec3(0.0);
    result += ambient;

    for(int i=0; i< directional_light_size; i++){
        result += calc_dir_light(directional_lights[i], normal, view_dir, position, diffuse_color, specular_color);
    }
    for(int i=0; i< point_light_size; i++){
        result += calc_point_light(point_lights[i], normal, view_dir, f_position, diffuse_color, specular_color);
    }
    frag_color = vec4(result, 1.0);
    
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    float threshold = 2.2;
    bright_color = vec4(brightness > threshold ? result : vec3(0.0), 1.0);
}

vec3 calc_dir_light(Light light, vec3 normal, vec3 view_dir, vec4 frag_pos, vec3 diffuse_color, vec3 specular_color)
{
    vec3 light_dir = normalize(-light.direction);
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diff * light.color * diffuse_color;

    vec3 halfway_dir = normalize(light_dir + view_dir);
    float spec = pow(max(dot(normal, halfway_dir), 0.0), shininess);
    vec3 specular = spec * light.color * specular_color;

    vec3 result = (diffuse + specular) * light.intensity;

    float shadow = calc_dir_shadow(normal, light_dir);
    result = result * (1.0 - shadow);
    return result;

}

vec3 calc_point_light(Light light, vec3 normal, vec3 view_dir, vec3 frag_pos,  vec3 diffuse_color, vec3 specular_color){
    vec3 light2frag = light.position - frag_pos;
    vec3 light_dir = normalize(light2frag);
    float d = length(light2frag);
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diff * light.color * diffuse_color;

    vec3 halfway_dir = normalize(light_dir + view_dir);
    float spec = pow(max(dot(normal, halfway_dir), 0.0), shininess);
    vec3 specular = spec * light.color * specular_color;

    float attenuation = 1.0 / (d*d) ;

    return (diffuse + specular) * attenuation * light.intensity;
}

float calc_dir_shadow(vec3 normal, vec3 light_dir){
    vec4 frag_position = texture(light_space_map, f_texcoord);
    vec3 proj_coords = frag_position.xyz / frag_position.w;
    // transform to [0,1] range
    proj_coords = proj_coords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closest_depth = texture(shadow_map, proj_coords.xy).r; 
    // get depth of current fragment from light's perspective
    float current_depth = proj_coords.z;

    bias = max(0.05 * (1.0 - dot(normal, light_dir)), bias);

    // check whether current frag pos is in shadow
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(light_space_map, 0);
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







