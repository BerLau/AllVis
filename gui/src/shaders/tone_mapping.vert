/*
vertex shader for tone mapping
in: vec3 position, vec3 normal, vec3 tangent, vec2 texCoord
out: vec2 texcoord
uniform: none
*/
#version 420 core
out vec2 texcoord;
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texcoord;

void main()
{
    texcoord = v_texcoord;
    gl_Position = vec4(v_position, 1.0);
}