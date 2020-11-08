#version 410


in vec2 fTexcoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;


float radius = 1.0;
float bias = 0.025;

// tile noise texture over screen based on screen dimensions divided by noise size
const vec2 noiseScale = vec2(2560.0/4.0, 1440.0/4.0); 

uniform mat4 projection;
uniform mat4 view;

uniform vec3 SSAOKernel[16];

out vec4 FragColor;


void main()
{

    // get input for SSAO algorithm
    vec3 fragPos = texture(gPosition, fTexcoord).xyz;
    vec3 normal = texture(gNormal, fTexcoord).rgb * 2.0f - 1.0f;
    vec3 randomVec = normalize(texture(texNoise, fTexcoord * noiseScale).xyz);

	normal = mat3(view) * normal;

    // create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    // iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0;
    for(int i = 0; i < 16; ++i)
    {

		// get sample position
		vec3 _sample = TBN * SSAOKernel[i]; // from tangent to view-space
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
		//occlusion += (sampleDepth >= (_sample.z + bias) ? 1.0 : 0.0) * rangeCheck;     

		if (sampleDepth >= (_sample.z + bias))
		{
			occlusion += rangeCheck;
		}
    }

    occlusion = 1.0 - (occlusion / 16.0);
    
    FragColor = vec4(vec3(occlusion), 1.0);
    //FragColor = texture(texNoise, fTexcoord);
}