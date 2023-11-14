/*
vertex shader for the environment prefiltering map
out: vec3 frag_position
in: vec3 v_position
uniform: mat4 u_view, mat4 u_projection
*/
#version 420 core
layout (location = 0) in vec3 v_position;

out vec3 frag_position;

uniform mat4 u_view;
uniform mat4 u_projection;

void main(){
    frag_position = v_position;
    vec4 pos = u_projection * u_view * vec4(frag_position, 1.0);
    gl_Position = pos;

}