#version 330 core
out mat3 tbn;
out vec2 texcoord;
out vec3 frag_position;

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_tangent;
layout(location = 3) in vec2 v_texcoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat3 u_normal_matrix;

struct Material {
  vec3 color;
  float metallic;
  float roughness;
  float ao;

  sampler2D albedo_map;
  sampler2D metallic_map;
  sampler2D roughness_map;
  sampler2D ao_map;
  sampler2D normal_map;
  sampler2D height_map;

  bool has_albedo_map;
  bool has_metallic_map;
  bool has_roughness_map;
  bool has_ao_map;
  bool has_normal_map;
  bool is_emissive;
  bool has_height_map;
};
uniform Material u_material;
void main() {
  texcoord = v_texcoord;
  vec3 pos = v_position;
  if (u_material.has_height_map) {
    float height = texture(u_material.height_map, v_texcoord).r;
    pos += v_normal * height * 0.1;
  }
  vec4 pos_view = u_view * u_model * vec4(v_position, 1.0);
  frag_position = pos_view.xyz;
  gl_Position = u_projection * pos_view;
  vec3 t = normalize(u_normal_matrix * v_tangent);
  vec3 n = normalize(u_normal_matrix * v_normal);
  tbn = mat3(t, cross(t, n), n);
}