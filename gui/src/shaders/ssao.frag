#version 330 core
out vec4 frag_color;

in vec2 f_texcoord;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D noise;

uniform vec3 samples[64];

uniform vec2 noise_scale;

int kernel_size = 64;
float radius = 1.0;
float bias = 0.025;



uniform mat4 projection;

void main()
{
    vec3 frag_pos = texture(g_position, f_texcoord).xyz;
    vec3 normal = texture(g_normal, f_texcoord).xyz;
    vec3 random_vec = texture(noise, f_texcoord * noise_scale).xyz;
    vec3 tangent = normalize(random_vec - normal * dot(random_vec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;
    for(int i = 0; i < kernel_size; ++i)
    {
        vec3 sample_pos = TBN * samples[i].xyz;
        sample_pos = frag_pos + sample_pos * radius;

        vec4 offset = vec4(sample_pos, 1.0);
        offset = projection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        float sample_depth = texture(g_position, offset.xy).z;
        float range_check = smoothstep(0.0, 1.0, radius / abs(frag_pos.z - sample_depth));
        occlusion += (sample_depth >= sample_pos.z + bias ? 1.0 : 0.0) * range_check;
    }
    occlusion = 1.0 - (occlusion / kernel_size);

    frag_color = vec4(vec3(occlusion), 1.0);
}