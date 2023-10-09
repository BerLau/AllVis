#version 330 core

in vec4 f_position;

uniform vec3 u_light_position;
uniform float u_far_plane;

void main(){
    float light_distance = length(u_light_position - f_position.xyz);
    light_distance = light_distance / u_far_plane;
    // light_distance = clamp(light_distance, 0.0, 1.0);
    gl_FragDepth = light_distance;
}
