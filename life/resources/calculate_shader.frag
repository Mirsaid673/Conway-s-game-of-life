#version 330 core

out uint out_color;

in vec2 o_tex_coord;

uniform usampler2D tex;

uint popcount3(uint value)
{
    uint sum = 0u;
    for(uint i = 0u; i < 3u; i++)
    {
        sum += (value & (1u << i)) >> i;
    }
    return sum;
}

uint getBit(uint data, int bit)
{
    return (data & (1u << bit)) >> bit;
}

uint get3bitsValue(uint value, int bit)
{
    return getBit(value, bit - 1) + getBit(value, bit) + getBit(value, bit + 1);
}

uint setBit(uint res, uint sum, int bit)
{
    if(sum == 3u)
        return res | (1u << bit);
    if(getBit(res, bit) == 1u && sum == 2u)
        return res | (1u << bit);
    
    return res & ((1u << bit) ^ 255u);
}

vec2 texel_size;
void main()
{
    texel_size = vec2(1.0f) / vec2(textureSize(tex, 0));

    uint self = texture(tex, o_tex_coord).r;

    uint left = texture(tex, o_tex_coord - vec2(texel_size.x, 0)).r;
    uint right = texture(tex, o_tex_coord + vec2(texel_size.x, 0)).r;
    uint bottom = texture(tex, o_tex_coord - vec2(0, texel_size.y)).r;
    uint top = texture(tex, o_tex_coord + vec2(0, texel_size.y)).r;

    uint top_left = texture(tex, o_tex_coord + vec2(-texel_size.x, texel_size.y)).r;
    uint top_right = texture(tex, o_tex_coord + texel_size).r;
    uint bottom_left = texture(tex, o_tex_coord - texel_size).r;
    uint bottom_right = texture(tex, o_tex_coord + vec2(texel_size.x, -texel_size.y)).r;

    uint res = self;

    // bit 0

    uint sum = getBit(left, 7); // left
    sum += getBit(top_left, 7); // left top
    sum += getBit(bottom_left, 7); // left bottom

    sum += getBit(top, 0); // top
    sum += getBit(bottom, 0); // bottom

    sum += getBit(top, 1); // top right
    sum += getBit(self, 1); // right
    sum += getBit(bottom, 1); // top bottom

    res = setBit(res, sum, 0);

    //bit 1 - bit 6

    for(int i = 1; i <= 6; i++)
    {
        sum = 0u;
        sum += getBit(self, i + 1); // right
        sum += getBit(self, i - 1); // left
        sum += get3bitsValue(top, i); // top 3 bits
        sum += get3bitsValue(bottom, i); // top 3 bits
        res = setBit(res, sum, i);
    }

    sum = 0u;
    sum += getBit(top, 6); // left top
    sum += getBit(self, 6); // left
    sum += getBit(bottom, 6); // left bottom

    sum += getBit(top, 7); // top
    sum += getBit(bottom, 7); // bottom

    sum += getBit(right, 0); // right
    sum += getBit(top_right, 0); // top right
    sum += getBit(bottom_right, 0); // top bottom

    res = setBit(res, sum, 7);

    out_color = res;
}