#version 330 core
out float frag_color;

in vec2 f_texcoord;

uniform sampler2D ssao_texture;

void main()
{
    vec2 texel_size = 1.0 / textureSize(ssao_texture, 0);
    float result = 0.0;
    for(int x= -2; x<2; x++)
    {
        for(int y= -2; y<2; y++)
        {
            vec2 offset = vec2(x, y) * texel_size;
            result += texture(ssao_texture, f_texcoord.xy + offset).r;
        }
    }
    frag_color = result / 16.0;
}