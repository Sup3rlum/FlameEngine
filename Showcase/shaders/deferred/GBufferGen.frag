#version 450 core

layout (location = 0) out vec4 gDepth;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gSpecular;


in vec2 fTexCoord;
in mat3 TBN;



layout(binding=0) uniform sampler2D DiffuseMap;
layout(binding=1) uniform sampler2D NormalMap;
layout(binding=2) uniform sampler2D RoughnessMap;


vec3 PackNormal(vec3 n)
{
	return (normalize(n) + 1.0f) / 2.0f;
}

vec3 CalculateNormalMap()
{
    vec3 BumpMapNormal = texture(NormalMap, fTexCoord).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0f);
    vec3 NewNormal;

    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
    return NewNormal;
}


void main()
{    

	float clipDepth = gl_FragCoord.z;// - FragPos.z / FragPos.w;
    
/*	float dx = dFdx(clipDepth);
	float dy = dFdy(clipDepth);*/
	float moment2 = clipDepth * clipDepth;// + 0.25 * (dx * dx + dy * dy);

	gDepth = vec4(clipDepth, moment2, 0.0, 1.0);
    gNormal = vec4(PackNormal(CalculateNormalMap()), 1.0);
    gAlbedo = texture(DiffuseMap, fTexCoord);
	gSpecular = vec4(1);//texture(RoughnessMap, TexCoords);

}