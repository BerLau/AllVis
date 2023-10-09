#version 330 core

layout (location = 0) out vec4 g_position;
layout (location = 1) out vec4 g_normal;
layout (location = 2) out vec4 g_albedo_spec;
layout (location = 3) out vec4 pos_in_light_space;


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

in vec2 f_texcoord;
in vec3 f_position;
in mat3 f_tbn;
in vec4 f_frag_pos_light_space;

uniform Material material;
uniform float near_plane = 0.1;
uniform float far_plane = 100.0;
uniform mat4 u_view;

// const float gamma = 2.2;

float depth_in_linear(float depth)
{
    float z = depth*2.0-1.0;
    return (2.0*near_plane*far_plane)/(far_plane+near_plane-z*(far_plane-near_plane));
}
void main()
{
    vec3 diffuse_color = material.albedo;

    if(material.has_albedo_map){
        diffuse_color.rgb = texture(material.albedo_map, f_texcoord).rgb;
        // diffuse_color.rgb = pow(diffuse_color.rgb, vec3(gamma));
    }
    float spec = 0;
    if(material.has_metallic_map){
        spec = texture(material.metallic_map, f_texcoord).r;
    }
    else{
        spec = material.metallic;
    }
    g_albedo_spec = vec4(diffuse_color, spec);

    vec3 normal = vec3(0.0,0.0,1.0);
    if(material.has_normal_map){
        vec3 normal_tbn = normalize(texture(material.normal_map, f_texcoord).rgb*2.0-1.0);
        normal  = normalize((u_view * vec4(f_tbn*normal_tbn, 0.0)).xyz);
    }
    else{
        normal = normalize((u_view * vec4(f_tbn*normal, 0.0)).xyz);
    }
    g_normal = vec4(normal, 1.0);

    vec3 pos = f_position;

    float depth = depth_in_linear(pos.z);

    g_position = vec4(pos,depth);

    // float shadow_detph = f_frag_pos_light_space.z/f_frag_pos_light_space.w;
    pos_in_light_space = f_frag_pos_light_space;
}
