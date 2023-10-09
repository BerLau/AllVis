#version 330 core
layout (location =0) out vec4 FragColor;
layout (location =1) out vec4 BrightColor;

in vec3 f_color;
in vec3 f_position;

float gamma = 2.2;
const vec3 red = vec3(1.0, 0.0, 0.0);
const vec3 green = vec3(0.0, 1.0, 0.0);
const vec3 blue = vec3(0.0, 0.0, 1.0);

uniform float near_plane = 0.1;
uniform float far_plane = 100.0;

float depth_in_linear(float depth)
{
    float z = depth*2.0-1.0;
    return (2.0*near_plane*far_plane)/(far_plane+near_plane-z*(far_plane-near_plane));
}
void main()
{   
    // vec3 color = f_color;
    vec3 color = pow(f_color, vec3(gamma));
    FragColor = vec4(color, depth_in_linear(f_position.z));
    BrightColor = vec4(color*0.75, 1.0);

}