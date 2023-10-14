#version 330 core

out vec4 frag_color;

in vec3 frag_position;
in vec3 frag_normal;
in vec2 texcoord;
in vec3 test;

uniform vec3 u_light_position;
uniform vec3 u_light_color;
uniform vec3 u_view_position;
uniform vec3 u_color;


void main()
{
    // ambient
    float ambientStrength = 0.15;
    vec3 ambient = ambientStrength * u_light_color;

    // diffuse 
    vec3 norm = normalize(frag_normal);
    vec3 lightDir = normalize(u_light_position - frag_position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_light_color;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_view_position - frag_position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_light_color;  

    vec3 result = (ambient + diffuse + specular) * u_color;
    frag_color = vec4(result, 1.0);
}