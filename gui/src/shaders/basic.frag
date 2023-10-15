#version 330 core

out vec4 frag_color;

in vec3 frag_position;
in vec3 frag_normal;
in vec2 texcoord;
in vec3 test;

#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2
struct Light{
    vec3 position;
    vec3 direction;
    vec3 color;
    int type;
};

struct Material{
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
};

uniform Light u_light;
uniform vec3 u_view_position;
uniform Material u_material;


void main()
{
    // ambient
    float ambientStrength = u_material.ao;
    vec3 ambient = ambientStrength * u_light.color;

    // diffuse 
    vec3 norm = normalize(frag_normal);
    vec3 lightDir = normalize(u_light.position - frag_position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_light.color;

    // specular
    float specularStrength = u_material.metallic;
    vec3 viewDir = normalize(u_view_position - frag_position);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_light.color;  

    vec3 result = (ambient + diffuse + specular) * u_material.albedo;
    frag_color = vec4(result, 1.0);
}