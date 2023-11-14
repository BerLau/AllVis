#version 420 core
out vec3 texcoords;

layout (location = 0) in vec3 v_position;
// layout (location = 1) in vec3 v_normal;
// layout (location = 2) in vec3 v_tangent;
// layout (location = 3) in vec2 v_texcoord;

uniform mat4 u_projection;
uniform mat4 u_view;

void main()
{
    mat4 view = mat4(mat3(u_view));
    vec4 position = u_projection * view * vec4(v_position, 1.0);
    gl_Position = position.xyww;
    texcoords = v_position;
}
