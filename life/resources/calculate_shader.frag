#version 330 core

out uint out_color;

in vec2 o_tex_coord;

uniform usampler2D tex;

vec2 texel_size;

void main()
{
    texel_size = vec2(1) / vec2(textureSize(tex, 0));

    uint self = texture(tex, o_tex_coord).r;
    uint left = texture(tex, o_tex_coord - vec2(texel_size.x, 0)).r;
    uint right = texture(tex, o_tex_coord + vec2(texel_size.x, 0)).r;
    uint bottom = texture(tex, o_tex_coord - vec2(0, texel_size.y)).r;
    uint top = texture(tex, o_tex_coord + vec2(0, texel_size.y)).r;

    uint top_left = texture(tex, o_tex_coord + vec2(-texel_size.x, texel_size.y)).r;
    uint top_right = texture(tex, o_tex_coord + texel_size).r;
    uint bottom_left = texture(tex, o_tex_coord - texel_size).r;
    uint bottom_right = texture(tex, o_tex_coord + vec2(texel_size.x, -texel_size.y)).r;

    uint sum = left + right + top + bottom + top_right + top_left + bottom_left + bottom_right;
    
    uint ret_color = 0u;
    if(self == 1u)
    {
        if(sum == 2u || sum == 3u)
            ret_color = 1u;
    }
    else
    {
        if(sum == 3u)
            ret_color = 1u;
    }
    out_color = ret_color;
}