/*
vertex shader for pbr shading
in: vec3 position, vec3 normal, vec3 tangent, vec2 texCoord
out: mat3 tbn, vec3 fragPos, vec2 texCoord
uniform: mat4 model, mat4 view, mat4 projection, mat3 normalMatrix
*/

#version 420 core
out mat3 tbn;
out vec3 frag_position;
out vec2 frag_texcoord;

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_tangent;
layout(location = 3) in vec2 v_texcoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat3 u_normal_matrix;


void main() {
  frag_texcoord = v_texcoord;
  vec3 pos = v_position;
  vec4 pos_view = u_view * u_model * vec4(pos, 1.0);
  frag_position = pos_view.xyz;
  gl_Position = u_projection * pos_view;
  vec3 t = normalize(u_normal_matrix * v_tangent);
  vec3 n = normalize(u_normal_matrix * v_normal);
  tbn = mat3(t, cross(t, n), n);
}