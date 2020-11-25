#version 450

in vec2 fTexcoord;

out vec4 fragColor;

uniform sampler2D Texture;
uniform vec4 Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

uniform float BorderRadius = 20.0f;
uniform vec2 Dimensions;


float sdRoundRect(vec2 pos, vec2 ext, float r) 
{

  return length(max(abs(pos) - ext + vec2(r) , 0.0)) - r;
}



void main()
{

   
    vec2 pos = fTexcoord * Dimensions;

    float dist = sdRoundRect(pos - Dimensions / 2.0, Dimensions / 2.0, BorderRadius);

	fragColor = texture(Texture, fTexcoord) * Color;

    fragColor.a *= smoothstep(1, 0, dist);
}