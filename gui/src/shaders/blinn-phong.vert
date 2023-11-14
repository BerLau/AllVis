/*
vertex shader for Blinn-Phong shading
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
layout(location = 3) in vec2 v_texCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat3 u_normal_matrix;

void main() {
  texcoord = v_texCoord;
  vec3 pos_local = v_position;
  vec4 pos_world = u_model * vec4(pos_local, 1.0);
  vec4 pos_view = u_view * pos_world;
  gl_Position = u_projection * pos_view;

  vec3 t = normalize(u_normal_matrix * v_tangent);
  vec3 n = normalize(u_normal_matrix * v_normal);
  tbn = mat3(t, cross(t, n), n);
}
