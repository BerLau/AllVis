#version 330 core

out vec3 frag_position;
out vec3 frag_normal;
out vec2 texcoord;
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
    frag_position = vec3(u_model * vec4(v_pos, 1.0));
    frag_normal = u_normal_matrix * v_normal;
    gl_Position = u_projection * u_view * u_model * vec4(v_pos, 1.0);
}
