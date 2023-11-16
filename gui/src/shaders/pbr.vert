/*
vertex shader for pbr shading
in: vec3 position, vec3 normal, vec3 tangent, vec2 texCoord
out: mat3 tbn, vec3 fragPos, vec2 texCoord
uniform: mat4 model, mat4 view, mat4 projection, mat3 normalMatrix
*/

#version 420 core
out mat3 tbn;
out vec3 frag_position;
out vec2 texcoord;

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_tangent;
layout(location = 3) in vec2 v_texcoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat3 u_normal_matrix;

struct Material {
  vec3 albedo;
  vec3 emissive;
  float metallic;
  float roughness;
  float ao;
  float height_scale;

  sampler2D albedo_map;
  sampler2D metallic_map;
  sampler2D roughness_map;
  sampler2D ao_map;
  sampler2D normal_map;
  sampler2D height_map;
  sampler2D emissive_map;

  float emissive_intensity;
  float emissive_texture_factor;
  float normal_texture_factor;
  float height_texture_factor;
  float albedo_texture_factor;
  float metallic_texture_factor;
  float roughness_texture_factor;
  float ao_texture_factor;
};

uniform Material u_material;

void main() {
  texcoord = v_texcoord;
  vec3 pos = v_position;
  float tex_height = texture(u_material.height_map, v_texcoord).r - 0.5;
  float height_off = mix(0.0, tex_height, u_material.height_texture_factor);
  pos += height_off * v_normal * u_material.height_scale;
  vec4 pos_view = u_view * u_model * vec4(pos, 1.0);
  frag_position = pos_view.xyz;
  gl_Position = u_projection * pos_view;
  vec3 t = normalize(u_normal_matrix * v_tangent);
  vec3 n = normalize(u_normal_matrix * v_normal);
  tbn = mat3(t, cross(t, n), n);
}