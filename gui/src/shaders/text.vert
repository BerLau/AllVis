#version 330 core
layout (location = 0) in vec4 pos_tex;
out vec2 TexCoords;
uniform mat4 projection;
void main()
{
   gl_Position = projection * vec4(pos_tex.xy, 0.0, 1.0);
   TexCoords = pos_tex.zw;
}

