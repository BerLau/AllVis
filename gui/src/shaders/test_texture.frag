#version 330 core
out vec4 frag_color;
in vec2 f_tex;
uniform sampler2D tex;

void main() {
    frag_color = texture(tex, f_tex);
}