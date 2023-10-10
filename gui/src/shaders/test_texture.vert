#version 330 core
out vec2 f_tex;
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec2 v_tex;

// uniform mat4 projection;

void main()
{
    gl_Position = vec4(v_pos, 1.0);
    f_tex = v_tex;
}