#version 330 core
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texcoord;
layout (location = 2) in vec3 v_normal;
layout (location = 3) in vec3 v_tangent;
layout (location = 4) in vec3 v_bitangent;

out vec2 f_texcoord;
out vec3 f_position;
out mat3 f_tbn;
out vec4 f_frag_pos_light_space;

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

uniform Material material;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat3 u_normal_mat;
uniform mat4 u_light_space_mat;

void main()
{
    vec3 T = normalize(u_normal_mat * v_tangent);
    vec3 N = normalize(u_normal_mat * v_normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    mat3 TBN = mat3(T, B, N);

    f_tbn = TBN;
    f_texcoord = v_texcoord;
    vec4 pos = vec4(v_position, 1.0);
    if(material.has_height_map){
        pos.xyz += v_normal * texture(material.height_map, v_texcoord).r;
    }
    pos =  u_model * pos;
    f_frag_pos_light_space = u_light_space_mat * pos;
    pos = u_view * pos;
    f_position = pos.xyz;
    gl_Position = u_projection * pos;
}