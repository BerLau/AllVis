#version 330 core
out vec2 texcoord;
out vec3 frag_position;
out vec3 frag_normal;

layout (location = 0) in vec3 v_position;
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
    vec4 pos_view = u_view * u_model * vec4(v_position, 1.0);
    frag_position = pos_view.xyz;
    frag_normal = normalize(u_normal_matrix * v_normal);
    gl_Position = u_projection * pos_view;
}