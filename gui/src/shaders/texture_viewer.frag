#version 330 core
out vec4 FragColor;
in vec2 f_texcoord;

#define CHANNEL_R 1
#define CHANNEL_G 2
#define CHANNEL_B 4
#define CHANNEL_A 8
#define CHANNEL_RGB 7
#define CHANNEL_RGBA 15

uniform int u_mode = CHANNEL_RGB;
uniform sampler2D u_texture;


void main()
{
    vec4 color = texture(u_texture, f_texcoord);
    if(u_mode ==  CHANNEL_R){
        color = vec4(color.r, color.r, color.r, 1.0);
    }
    else if(u_mode ==  CHANNEL_G){
        color = vec4(color.g, color.g, color.g, 1.0);
    }
    else if(u_mode ==  CHANNEL_B){
        color = vec4(color.b, color.b, color.b, 1.0);
    }
    else if(u_mode ==  CHANNEL_A){
        color = vec4(color.a, color.a, color.a, 1.0);
    }
    else if(u_mode ==  CHANNEL_RGB){
        color = vec4(color.r, color.g, color.b, 1.0);
    }
    else if(u_mode ==  CHANNEL_RGBA){
        color = vec4(color.r, color.g, color.b, color.a);
    }
    FragColor = color;
}