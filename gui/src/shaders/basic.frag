#version 330 core
#define LIGHT_MAX 4

out vec4 frag_color;

in vec3 frag_position;
in vec3 frag_normal;
in vec2 texcoord;
in vec3 test;

#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2
struct Light
{
    vec3 position;
    vec3 direction;
    vec3 color;
    int type;
};

struct Material
{
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
};

uniform int u_light_num;
uniform Light[LIGHT_MAX] u_lights;
uniform vec3 u_view_position;
uniform Material u_material;

float near = 0.1;
float far = 100.0;

void main()
{
    float ambient_intensity = u_material.ao;
    float specular_intensity = u_material.metallic;
    float diffuse_intensity = 1.0 - u_material.metallic;
    vec3 norm = normalize(frag_normal);
    vec3 view_dir = normalize(u_view_position - frag_position);

    vec3 result = vec3(0.0);

    for (int i = 0; i < u_light_num; ++i)
    {
        vec3 ambient = ambient_intensity * u_lights[i].color;

        // diffuse
        vec3 light_dir = normalize(u_lights[i].position - frag_position);
        float diff = max(dot(norm, light_dir), 0.0);
        vec3 diffuse = diffuse_intensity * diff * u_lights[i].color;

        // specular
        vec3 halfway = normalize(light_dir + view_dir);
        float spec = pow(max(dot(norm, halfway), 0.0), u_material.roughness*10);
        vec3 specular = specular_intensity * spec * u_lights[i].color;

        result += (ambient + diffuse + specular) * u_material.albedo;
    }

    frag_color = vec4(result, 1.0);
}