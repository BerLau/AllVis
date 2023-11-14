#version 420 core

out vec3 frag_position;
out vec3 frag_pos_tbn;
out vec2 texcoord;
out mat3 TBN;

out vec3 test;

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec3 v_tangent;
layout (location = 3) in vec2 v_texcoord;


uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat3 u_normal_matrix;
// uniform vec3 u_view_position;

void main()
{
    texcoord = v_texcoord;
    vec4 pos_world = u_model * vec4(v_pos, 1.0);
    vec4 pos_view = u_view * pos_world;
    frag_position = pos_view.xyz;

    vec3 T = normalize(u_normal_matrix * v_tangent);
    vec3 N = normalize(u_normal_matrix * v_normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    TBN = transpose(mat3(T, B, N));

    frag_pos_tbn = TBN * frag_position;


    gl_Position = u_projection * pos_view;
    
}
