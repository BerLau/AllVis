/*
vertex shader for basic 2D rendering
out: vec2 texcoord
in: vec2 v_position, vec2 v_texcoord
uniform: mat4 u_projection
*/
#version 420 core
layout(location = 0) in vec2 v_position;
layout(location = 1) in vec2 v_texcoord;
out vec2 texcoord;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * vec4(v_position, 0.0, 1.0);
    texcoord = v_texcoord;
}