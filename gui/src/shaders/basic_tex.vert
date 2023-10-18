#version 330 core

out vec3 frag_position;
out vec3 frag_normal;
out vec2 texcoord;
out mat3 TBN;
out vec3 view_pos;

out vec3 test;

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec3 v_tangent;
layout (location = 3) in vec2 v_texcoord;


uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat3 u_normal_matrix;

void main()
{
    texcoord = v_texcoord;
    vec4 pos_world = u_model * vec4(v_pos, 1.0);
    vec4 pos_view = u_view * pos_world;
    frag_position = pos_view.xyz;
    vec3 T = normalize(u_view * vec4(v_tangent, 0.0)).xyz;
    vec3 N = normalize(u_normal_matrix * v_normal);
    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N));
    frag_normal = TBN * v_normal;
    gl_Position = u_projection * pos_view;
    
}
