#version 450 core
out float FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;
uniform sampler2D texSample;


float radius = 2.0;
float bias = 0.0;

// tile noise texture over screen based on screen dimensions divided by noise size
const vec2 noiseScale = vec2(2560.0/4.0, 1440.0/4.0); 

uniform mat4 projection;

void main()
{
    // get input for SSAO algorithm
    vec3 fragPos = texture(gPosition, TexCoords).xyz;
    vec3 normal = texture(gNormal, TexCoords).rgb * 2.0f - 1.0f;
    vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);

    // create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    // iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0;
    for(int i = 0; i < 4; ++i)
    {
	    for(int j = 0; j < 4; ++j)
		{
			vec3 _tSample = texture(texSample, vec2(i,j) / 4.0f).rgb;

			// get sample position
			vec3 _sample = TBN * _tSample; // from tangent to view-space
			_sample = fragPos + _sample * radius; 
			
			// project sample position (to sample texture) (to get position on screen/texture)
			vec4 offset = vec4(_sample, 1.0);
			offset = projection * offset; // from view to clip-space
			offset.xyz /= offset.w; // perspective divide
			offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        
			// get sample depth
			float sampleDepth = texture(gPosition, offset.xy).z; // get depth value of kernel sample
			
			// range check & accumulate
			float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
			occlusion += (sampleDepth >= (_sample.z + bias) ? 1.0 : 0.0) * rangeCheck;     
		}
    }
    occlusion = 1.0 - (occlusion / 16);
    
    FragColor = occlusion;
}