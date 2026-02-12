#version 450



const float KERNEL_RADIUS = 3;
  
uniform float Sharpness = 1;
uniform vec2  Direction; // either set x to 1/width or y to 1/height

uniform sampler2D Texture;

in vec2 fTexcoord;

out vec4 fragColor;

#ifndef AO_BLUR_PRESENT
#define AO_BLUR_PRESENT 1
#endif


//-------------------------------------------------------------------------

float BlurFunction(vec2 uv, float r, float center_c, float center_d, inout float w_total)
{
    vec2  aoz = texture2D(Texture, uv ).xy;
    float c = aoz.x;
    float d = aoz.y;
    
    const float BlurSigma = float(KERNEL_RADIUS) * 0.5;
    const float BlurFalloff = 1.0 / (2.0*BlurSigma*BlurSigma);
    
    float ddiff = (d - center_d) * Sharpness;
    float w = exp2(-r*r*BlurFalloff - ddiff*ddiff);
    w_total += w;

    return c*w;
}

void main()
{

    vec2 g_Direction = Direction / vec2(2560.0, 1440.0);

    vec2  aoz = texture2D(Texture, fTexcoord ).xy;
    float center_c = aoz.x;
    float center_d = aoz.y;
    
    float c_total = center_c;
    float w_total = 1.0;
    
    for (float r = 1; r <= KERNEL_RADIUS; ++r)
    {
        vec2 uv =  fTexcoord + g_Direction * r;
        c_total += BlurFunction(uv, r, center_c, center_d, w_total);  
    }
    
    for (float r = 1; r <= KERNEL_RADIUS; ++r)
    {
        vec2 uv =  fTexcoord - g_Direction * r;
        c_total += BlurFunction(uv, r, center_c, center_d, w_total);  
    }
  
    fragColor = vec4(c_total/w_total);

}