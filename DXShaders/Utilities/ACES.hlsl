
float3 ACESFilm(float3 x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return saturate((x * (a * x + b)) / (x * (c * x + d) + e));
}


float3 LinearToSRGB(float3 color)
{
    return pow(color, 1.0 / 2.2);
}

float3 ReinhardtTonemap(float3 color)
{
    return color / (color + float3(1,1,1));
}


float3 ExposureTonemap(float3 color, float exposure)
{
    return float3(1, 1, 1) - exp(-color * exposure);
}

float3 postProcessColor(float2 seed, float3 input)
{
    input = ACESFilm(input);

    float rand = frac(sin(dot(
        seed, float2(12.9898, 78.233) * 2.0)) * 43758.5453);
    input = 255 * saturate(pow(input, 1 / 2.2));
    input = rand.xxx < (input - floor(input)) ? ceil(input) : floor(input);

    return input / 255;
}