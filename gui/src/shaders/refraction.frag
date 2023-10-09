#version 330 core

out vec4 FragColor;

in vec3 f_position;
in vec3 f_normal;

uniform vec3 view_pos;
uniform samplerCube skybox;

void main()
{
    vec3 I = normalize(f_position - view_pos);
    vec3 R = refract(I, normalize(f_normal), 1.0 / 1.52);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
    FragColor *= 0.9;
    // FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}