#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in vec4 f_light_dir[];

uniform mat4 u_projection;

void main(){
    gl_Position =  u_projection* vec4(gl_in[1].gl_Position + f_light_dir[1]*0.02);
    EmitVertex();
    gl_Position =  u_projection * vec4(gl_in[1].gl_Position + f_light_dir[1]*0.12);
    EmitVertex();
    EndPrimitive();
    
    gl_Position =  u_projection* vec4(gl_in[2].gl_Position + f_light_dir[2]*0.02);
    EmitVertex();
    gl_Position =  u_projection * vec4(gl_in[2].gl_Position + f_light_dir[2]*0.12);
    EmitVertex();
    EndPrimitive();
}