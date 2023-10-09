#version 330 core
layout (location =0) out vec4 FragColor;
layout (location =1) out vec4 BrightColor;

uniform vec3 u_color;
float gamma = 2.2;
void main()
{
    vec3 color = u_color;
    if(gl_FrontFacing){
        FragColor = vec4(color, 1.0);
        BrightColor = vec4(color*0.9, 1.0);
    }
    else{
        FragColor = vec4(color*0.5, 1.0);
        BrightColor = vec4(0, 0, 0, 1.0);
    }

}