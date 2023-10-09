#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texcoord;
layout (location = 2) in vec3 v_normal;
layout (location = 3) in vec3 v_tangent;
layout (location = 4) in vec3 v_bitangent;

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

out OUT{
    mat3 tbn;
    vec3 position;
    vec2 texcoord;
    vec4 pos_in_light_space;
} out_data;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform Material material;
uniform vec3 view_pos;
uniform mat3 u_normal_mat;
uniform mat4 u_light_space_mat;

void main(){

    out_data.texcoord = v_texcoord;
    vec3 T = normalize(u_normal_mat * v_tangent);
    vec3 N = normalize(u_normal_mat * v_normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    // out_data.tbn = transpose(mat3(T, B, N));
    out_data.tbn = mat3(T, B, N);
    vec4 pos = vec4(v_position,1.0);
    // if(material.has_height_map){
    //     pos.xyz += v_normal * texture(material.height_map, v_texcoord).r;
    // }
    out_data.pos_in_light_space = u_light_space_mat * u_model * pos;

    pos = u_view * u_model * pos;
    
    out_data.position = pos.xyz;

    gl_Position = u_projection * pos;

}
