#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texcoord;
layout (location = 2) in vec3 v_normal;
layout (location = 3) in vec3 v_tangent;
layout (location = 4) in vec3 v_bitangent;

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

out OUT{
    vec3 position;
    vec2 texcoord;
    vec4 pos_in_light_space;
    vec3 tspace_view_pos;
    vec3 tspace_frag_pos;
    mat3 tbn;
} v_out;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat3 u_normal_mat;
uniform mat4 u_light_space_mat;
uniform vec3 u_view_pos;
uniform Material material;

void main(){
    vec3 pos = v_position;
    if(material.has_height_map){
        pos += v_normal * (texture(material.height_map, v_texcoord).r);
    }
    v_out.position = vec3(u_model * vec4(pos, 1.0f));
    v_out.texcoord = v_texcoord;
    v_out.pos_in_light_space = u_light_space_mat * vec4(v_out.position, 1.0f);
    // v_out.view_pos = vec3(u_view * vec4(u_view_pos, 1.0f));

    vec3 T = normalize(u_normal_mat * v_tangent);
    vec3 N = normalize(u_normal_mat * v_normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N));
    v_out.tbn = TBN;

    v_out.tspace_view_pos = TBN * u_view_pos;
    v_out.tspace_frag_pos = TBN * v_out.position;

    gl_Position = u_projection * u_view * u_model * vec4(pos, 1.0f);

}