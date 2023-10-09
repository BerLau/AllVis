#version 330 core

layout (location = 0) in vec3 v_position;

out vec3 texture_coords;

uniform mat4 u_projection;
uniform mat4 u_view;

void main()
{
    vec4 pos = u_projection * u_view * vec4(v_position, 1.0);
    texture_coords = v_position;
    gl_Position = pos.xyww;
}
