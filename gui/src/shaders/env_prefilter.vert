#version 330 core
layout (location = 0) in vec3 v_position;

out vec3 f_position;

uniform mat4 u_view;
uniform mat4 u_projection;

void main(){
    f_position = v_position;
    vec4 pos = u_projection * u_view * vec4(f_position, 1.0);
    gl_Position = pos;

}