#version 330 core
out vec4 FragColor;

uniform vec3 u_color;
float gamma = 2.2;
void main()
{
    vec3 color = pow(u_color, vec3(gamma));
    FragColor = vec4(color, 1.0);
}