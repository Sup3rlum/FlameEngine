#version 450
out float FragColor;

in vec2 TexCoords;

uniform sampler2D ssaoInput;

float gaussKernel[25] =
{
	
0.003765,	0.015019,	0.023792,	0.015019,	0.003765,
0.015019,	0.059912,	0.094907,	0.059912,	0.015019,
0.023792,	0.094907,	0.150342,	0.094907,	0.023792,
0.015019,	0.059912,	0.094907,	0.059912,	0.015019,
0.003765,	0.015019,	0.023792,	0.015019,	0.003765
};

void main() 
{
    vec2 texelSize = 1.0 / vec2(textureSize(ssaoInput, 0));
    float result = 0.0;
    for (int x = 0; x < 5; ++x) 
    {
        for (int y = 0; y < 5; ++y) 
        {
            vec2 offset = vec2(float(x-2), float(y-2)) * texelSize;
            result += texture(ssaoInput, TexCoords + offset).r * gaussKernel[x+y*5];
        }
    }

    FragColor = result;
}  