/*
fragment shader for convert equitangular cubemap to 6 faces cubemap
out: vec4 frag_color, vec4 hilight_color
in: vec3 frag_position
uniform: sampler2D equi_cubemap, float gamma, float exposure, bool is_hdr
*/

#version 420 core
layout(location = 0) out vec4 frag_color;

in vec3 frag_position;

uniform sampler2D u_equirectangular_map;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v) {
  vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
  uv *= invAtan;
  uv += 0.5;
  return uv;
}

vec3 tone_mapping(vec3 color) { return color / (color + vec3(1.0)); }
vec3 gamma_correction(vec3 color) { return pow(color, vec3(1.0 / 2.2)); }

void main() {
  vec2 uv = SampleSphericalMap(normalize(frag_position));
  vec3 color = texture(u_equirectangular_map, uv).rgb;
  frag_color = vec4(color, 1.0);
}