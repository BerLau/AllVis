#version 330 core
out vec4 FragColor;

in vec2 f_texcoord;

uniform sampler2D u_image;
uniform sampler2D u_bloom;
uniform float u_exposure;

const float a = 2.51;
const float b = 0.03;
const float c = 2.43;
const float d = 0.59;
const float e = 0.14;
vec3 aces_tone_mapping(vec3 x)
{
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}
const float gamma = 2.2;

void main()
{
    vec4 color1 = texture(u_image, f_texcoord);
    vec4 color2 = texture(u_bloom, f_texcoord);
    vec3 hdrColor = color1.rgb;
    vec3 bloomColor = color2.rgb;
    hdrColor = hdrColor + bloomColor;
    hdrColor =  aces_tone_mapping(hdrColor * u_exposure);
    FragColor = vec4(pow(hdrColor, vec3(1.0 / gamma)), 1.0);
}