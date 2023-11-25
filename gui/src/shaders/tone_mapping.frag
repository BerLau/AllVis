/*
fragment shader for tone mapping
in: vec2 texcoord
uniform sampler2D u_image
uniform float u_exposure
out: vec4 frag_color
*/

#version 420 core
out vec4 frag_color;

in vec2 texcoord;

uniform sampler2D u_image;
uniform float u_exposure;
uniform float u_gamma = 2.2;

vec3 aces_tone_mapping(vec3 x)
{
    return clamp((x * (2.51 * x + 0.03)) / (x * (2.43 * x + 0.59) + 0.14), 0.0, 1.0);
}

vec3 aces_filmic_tone_mapping(vec3 x)
{
    x = max(vec3(0.0), x - 0.004);
    return clamp((x * (6.2 * x + 0.5)) / (x * (6.2 * x + 1.7) + 0.06), 0.0, 1.0);
}

vec3 aces_approx_tone_mapping(vec3 x)
{
    return clamp((x * (2.51 * x + 0.03)) / (x * (2.43 * x + 0.59) + 0.14), 0.0, 1.0);
}

vec3 aces_approx_filmic_tone_mapping(vec3 x)
{
    x = max(vec3(0.0), x - 0.004);
    return clamp((x * (6.2 * x + 0.5)) / (x * (6.2 * x + 1.7) + 0.06), 0.0, 1.0);
}

vec3 gamma_correct(vec3 x, float gamma)
{
    return pow(x, vec3(1.0 / gamma));
}

void main()
{
    vec3 color = texture(u_image, texcoord).rgb;
    // color = aces_tone_mapping(color * u_exposure);
    color = gamma_correct(color, u_gamma);
    frag_color = vec4(color, 1.0);
}