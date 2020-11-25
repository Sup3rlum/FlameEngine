#version 450


in vec2 fTexcoord;

uniform sampler2D gDepth;
uniform sampler2D gNormal;
uniform sampler2D texNoise;


float radius = 0.4;
float bias = 0.025;

// tile noise texture over screen based on screen dimensions divided by noise size
const vec2 noiseScale = vec2(2560.0/4.0, 1440.0/4.0); 

uniform mat4 Projection;
uniform mat4 InverseProjection;
uniform mat4 SceneView;



uniform int KernelSize = 32;

vec3 SSAOKernel[64] = vec3[64] 
(
vec3(0.0497709, -0.0447092, 0.0499634),
vec3(0.0150539, 0.0170748, 0.00231224),
vec3(-0.0459599, -0.0219068, 0.0361069),
vec3(0.0177858, -0.118221, 0.0528281),
vec3(0.0845117, 0.0902513, 0.0870322),
vec3(0.164332, 0.0788634, 0.0275194),
vec3(-0.00428153, -0.11422, 0.140113),
vec3(-0.000812446, -0.000460475, 0.00090779),
vec3(0.142593, -0.13292, 0.0723297),
vec3(0.124538, 0.102567, 0.107357),
vec3(-0.118035, 0.0757417, 0.0833537),
vec3(0.231189, -0.153459, 0.226167),
vec3(0.264216, -0.103806, 0.347895),
vec3(-0.0814443, -0.0190749, 0.280794),
vec3(-0.138066, -0.136941, 0.137615),
vec3(-0.200861, 0.127544, 0.15146),
vec3(-0.112191, 0.0920681, 0.0342291),
vec3(0.500727, 0.76484, 0.0751821),
vec3(-0.318871, -0.653462, 0.605687),
vec3(-0.635832, -0.012853, 0.648884),
vec3(-0.0834186, -0.263482, 0.154504),
vec3(0.0269169, -0.0409209, 0.0349006),
vec3(-0.0644607, -0.574783, 0.588433),
vec3(0.853213, -0.0724909, 1.2992),
vec3(0.72064, 1.19087, 1.00358),
vec3(0.00380138, 0.00435289, 0.00291948),
vec3(-1.04422, 0.652133, 1.01377),
vec3(-0.0455603, -1.22447, 1.65702),
vec3(-0.782107, 0.361396, 2.35136),
vec3(-0.0296047, 0.0330335, 0.0313676),
vec3(-1.18954, 1.56034, 1.82451),
vec3(0.524107, 1.15858, 0.666014),
vec3(1.53202, -0.256291, 1.48585),
vec3(-1.90461, -1.80219, 1.44204),
vec3(-2.20747, -2.45647, 0.679533),
vec3(-0.5219, 1.03761, 0.892834),
vec3(-0.031024, -0.024213, 0.0492492),
vec3(-1.18525, -2.23405, 3.67097),
vec3(-2.80072, 3.92084, 1.10601),
vec3(-0.345062, 3.60288, 3.97912),
vec3(2.62713, -3.43375, 1.39627),
vec3(0.70297, 1.33605, 4.1371),
vec3(-1.69119, 1.54172, 3.62146),
vec3(2.00818, -0.852249, 2.9965),
vec3(0.695868, -2.9953, 1.95174),
vec3(-2.48131, -0.532875, 0.210983),
vec3(1.89287, 0.629384, 1.79951),
vec3(-0.594848, 0.747336, 0.750613),
vec3(-0.318985, -1.09518, 2.9639),
vec3(-1.93459, 2.69568, 0.070684),
vec3(2.17184, 3.15493, 0.598459),
vec3(0.413538, 5.25638, 2.25526),
vec3(1.14872, -4.28542, 0.932933),
vec3(3.15659, -3.27339, 2.69372),
vec3(5.32847, 4.64025, 7.39962),
vec3(-7.80915, 4.13733, 4.22697),
vec3(5.98489, 0.0796155, 1.65773),
vec3(5.18854, 0.0508004, 1.21329),
vec3(4.67617, 0.439139, 2.53699),
vec3(-1.18316, 7.3161, 0.806797),
vec3(12.4176, -0.039138, 1.43362),
vec3(6.54691, -11.0901, 0.0551814),
vec3(6.01402, -2.23166, 6.66578),
vec3(-6.4004, -9.82034, 2.69626)
);


out vec4 FragColor;




float GetDepth(vec2 tCoord)
{
	return texture(gDepth, tCoord).r * 2.0 - 1.0;
}

vec3 UnpackPosition(vec2 tCoord)
{

	float d = GetDepth(tCoord);


	vec4 tray = InverseProjection * vec4(tCoord * 2.0 - 1.0, d, 1.0);

	return tray.xyz / tray.w;
}



void main()
{

    // get input for SSAO algorithm
    vec3 fragPos = UnpackPosition(fTexcoord);
    vec3 normal = texture(gNormal, fTexcoord).rgb * 2.0f - 1.0f;
    vec3 randomVec = normalize(texture(texNoise, fTexcoord * noiseScale).xyz);

	normal = mat3(SceneView) * normal;

    // create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);


    // iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0;
    for(int i = 0; i < KernelSize; ++i)
    {

		// get sample position
		vec3 _sample = TBN * SSAOKernel[i]; 
		_sample = fragPos + _sample * radius; 
		
		// project sample position (to sample texture) (to get position on screen/texture)
		vec4 offset = vec4(_sample, 1.0);
		offset = Projection * offset; 
		offset.xyz /= offset.w; 
		offset.xyz = offset.xyz * 0.5 + 0.5; 
        
		// get sample depth
		float sampleDepth = UnpackPosition(offset.xy).z;
		
		// range check & accumulate
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth)); 

		if (sampleDepth >= (_sample.z + bias))
		{
			occlusion += rangeCheck;
		}
    }

    occlusion = 1.0 - (occlusion / float(KernelSize));
    
    //FragColor = vec4(occlusion,0.0,0.0, 1.0);
	FragColor = vec4(1.0);
}