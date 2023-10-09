#version 330 core
in vec2 TexCoords;
out vec4 FragColor;
uniform sampler2D u_text;
uniform vec3 u_color;
void main()
{
   vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_text, TexCoords).r);
   FragColor = vec4(u_color, 1.0) * sampled;
}