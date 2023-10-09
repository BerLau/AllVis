#version 330 core

layout (location = 0) in vec3 v_position;

uniform mat4 u_model;
uniform mat4 u_light_space_mat;

void main()
{
    gl_Position = u_light_space_mat * u_model * vec4(v_position, 1.0);
}