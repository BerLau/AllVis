/*
vertext shader for convert equitangular cubemap to 6 faces cubemap
out: vec3 frag_position
in: vec3 v_position
uniform: mat4 u_view, u_projection
*/
#version 420 core
out vec3 frag_position;
layout (location = 0) in vec3 v_position;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    frag_position = v_position;
    gl_Position = u_projection * u_view * vec4(v_position, 1.0);
}