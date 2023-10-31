#version 330 core

#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

layout (location = 0) out vec4 frag_color;
layout (location = 1) out vec4 other_color;

in vec2 texcoord;
in vec3 frag_position;
in vec3 frag_normal;
uniform mat4 u_view;
uniform vec3 u_color;


void main()
{
    vec3 view_dir = normalize(-frag_position);
    vec3 view_normal = normalize(u_view * vec4(frag_normal, 0.0)).xyz;
    float diffuse = max(dot(view_normal, view_dir), 0.0);
    frag_color = vec4(u_color * diffuse, 1.0);
    other_color = vec4(1.0, 0.0, 0.0, 1.0);
}
