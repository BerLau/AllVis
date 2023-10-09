#version 330 core
layout (location = 0) in vec3 v_position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform vec3 u_direction;

out vec4 f_light_dir;

void main()
{
    f_light_dir =  vec4(u_direction, 0.0);
    gl_Position = u_view * u_model * vec4(v_position, 1.0);
}
