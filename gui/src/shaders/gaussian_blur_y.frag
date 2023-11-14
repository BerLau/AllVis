/*
fragement shader for gaussian blur in y direction
in: texcoord
out: frag_color
*/
#version 420 core
out vec4 frag_color;

in vec2 texcoord;

const int W_SIZE = 3;
uniform sampler2D u_texture;
uniform vec2 u_resolution;
uniform float offsets[W_SIZE] = float[](0.0, 1.5, 3.5);
uniform float weights[W_SIZE] =
    float[](0.2270270270, 0.3162162162, 0.0702702703);

void main() {
  float tex_off = 1.0 / u_resolution.y;
  vec3 result = texture(u_texture, f_texcoord).rgb * weights[0];
  for (int i = 1; i < W_SIZE; i++) {
    vec2 offset = vec2(0.0, offsets[i]) * tex_off;
    result += texture(u_texture, f_texcoord - offset).rgb * weights[i];
    result += texture(u_texture, f_texcoord + offset).rgb * weights[i];
  }
  frag_color = vec4(result, 1.0);
}