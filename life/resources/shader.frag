#version 330 core

out vec4 out_color;

in vec2 o_tex_coord;

uniform vec3 alive_color;
uniform vec3 dead_color;

uniform usampler2D tex;

void main()
{
    float tex_size_x = textureSize(tex, 0).x * 8.0f;
    highp int x_coord = int(tex_size_x * o_tex_coord.x);
    int bit = x_coord % 8;

    uint value = texture(tex, o_tex_coord).r;
    value &= (1u << bit);

    out_color.rgb = value != 0u ? alive_color : dead_color;
    out_color.a = 1;
}