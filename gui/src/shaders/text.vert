#version 420 core
layout (location = 0) in vec4 pos_tex;
out vec2 TexCoords;
uniform mat4 u_projection;
void main()
{
   gl_Position = u_projection * vec4(pos_tex.xy, 0.0, 1.0);
   TexCoords = pos_tex.zw;
}

