


float4 UintToRGBA8(uint val)
{
    return float4(float((val & 0x000000FF)),
    float((val & 0x0000FF00) >> 8U),
    float((val & 0x00FF0000) >> 16U),
    float((val & 0xFF000000) >> 24U));
}

uint RGBA8ToUint(float4 val)
{
    return (uint(val.w) & 0x000000FF) << 24U |
    (uint(val.z) & 0x000000FF) << 16U |
    (uint(val.y) & 0x000000FF) << 8U |
    (uint(val.x) & 0x000000FF);
}