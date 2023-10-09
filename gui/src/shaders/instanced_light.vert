#version 330 core
layout (location = 0) in vec3 v_position;
layout (location = 2) in vec3 i_color;
layout (location = 3) in mat4 i_model;

uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 f_color;
out vec3 f_position;

void main()
{
    f_color = i_color;
    vec4 pos = u_view*i_model*vec4(v_position, 1.0f);
    f_position = pos.xyz;
    gl_Position = u_projection * pos;
}
