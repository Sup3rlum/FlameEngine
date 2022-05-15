
#define PI 3.141592

float2 rsi(float3 r0, float3 rd, float sr)
{
    // ray-sphere intersection that assumes
    // the sphere is centered at the origin.
    // No intersection when result.x > result.y
    float a = dot(rd, rd);
    float b = 2.0 * dot(rd, r0);
    float c = dot(r0, r0) - (sr * sr);
    float d = (b * b) - 4.0 * a * c;
    if (d < 0.0)
        return float2(1e5, -1e5);
    
    return float2(
        (-b - sqrt(d)) / (2.0 * a),
        (-b + sqrt(d)) / (2.0 * a)
    );
}


float3 AtmosphericScattering(
    float3 r,
    float3 r0,
    float3 pSun,
    float iSun,
    float rPlanet,
    float rAtmos,
    float3 kRlh,
    float kMie,
    float shRlh,
    float shMie,
    float g,
    int iSteps,
    int jSteps)
{
    // Normalize the sun and view directions.
    pSun = normalize(pSun);
    r = normalize(r);

    // Calculate the step size of the primary ray.
    float2 p = rsi(r0, r, rAtmos);
    if (p.x > p.y)
        return float3(0, 0, 0);
    p.y = min(p.y, rsi(r0, r, rPlanet).x);
    float iStepSize = (p.y - p.x) / float(iSteps);

    // Initialize the primary ray time.
    float iTime = 0.0;

    // Initialize accumulators for Rayleigh and Mie scattering.
    float3 totalRlh = float3(0, 0, 0);
    float3 totalMie = float3(0, 0, 0);

    float2 iOd = 0;
    
    // Calculate the Rayleigh and Mie phases.
    float mu = dot(r, pSun);
    float mumu = mu * mu;
    float gg = g * g;
    float pRlh = 3.0 / (16.0 * PI) * (1.0 + mumu);
    float pMie = 3.0 / (8.0 * PI) * ((1.0 - gg) * (mumu + 1.0)) / (pow(1.0 + gg - 2.0 * mu * g, 1.5) * (2.0 + gg));
    
     
    float2 odStepPre = exp(-1.0 / float2(shRlh, shMie));
    
    // Sample the primary ray.
    for (int i = 0; i < iSteps; i++)
    {

        // Calculate the primary ray sample position.
        float3 iPos = r0 + r * (iTime + iStepSize * 0.5);

        // Calculate the height of the sample.
        float iHeight = length(iPos) - rPlanet;
        
        float2 odStep = pow(odStepPre, iHeight) * iStepSize;
       
        iOd += odStep;
        
        // Calculate the step size of the secondary ray.
        float jStepSize = rsi(iPos, pSun, rAtmos).y / float(jSteps);

        // Initialize the secondary ray time.
        float jTime = 0.0;

        float2 jOd = 0;
        
        // Sample the secondary ray.
        for (int j = 0; j < jSteps; j++)
        {
            // Calculate the secondary ray sample position.
            float3 jPos = iPos + pSun * (jTime + jStepSize * 0.5);

            // Calculate the height of the sample.
            float jHeight = length(jPos) - rPlanet;
            
            jOd += pow(odStepPre, jHeight) * jStepSize;

            // Increment the secondary ray time.
            jTime += jStepSize;
        }

        // Calculate attenuation.
        float3 attn = exp(-(kMie * (iOd.y + jOd.y) + kRlh * (iOd.x + jOd.x)));
      
        totalRlh += odStep.x * attn;
        totalMie += odStep.y * attn;

        // Increment the primary ray time.
        iTime += iStepSize;

    }

    // Calculate and return the final color.
    return iSun * (pRlh * kRlh * totalRlh + pMie * kMie * totalMie);
}
