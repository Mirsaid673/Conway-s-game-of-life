#version 330 core

out vec4 out_color;

in vec2 o_tex_coord;

uniform vec3 alive_color;
uniform vec3 dead_color;

uniform usampler2D tex;

void main()
{
    uint value = texture(tex, o_tex_coord).r;

    out_color.rgb = value == 1u ? alive_color : dead_color;
    out_color.a = 1;
}