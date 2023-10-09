#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texcoord;
layout (location = 2) in vec3 v_normal;



out vec3 f_position;
out vec3 f_normal;


uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat3 u_normal_mat;

void main(){
    f_position = vec3(u_model * vec4(v_position, 1.0));
    f_normal = u_normal_mat * v_normal;
    gl_Position = u_projection * u_view * u_model* vec4(v_position, 1.0);
}



