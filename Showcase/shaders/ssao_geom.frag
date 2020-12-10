#version 450 core

layout (location = 0) out vec4 gDepth;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gSpecular;


in vec2 TexCoords;
in vec4 FragPos;
in vec3 Normal;
in vec3 Tangent;

in mat3 TBN;

uniform sampler2D Texture;
uniform sampler2D NormalMap;
uniform sampler2D RoughnessMap;


vec3 PackNormal(vec3 n)
{
	return (normalize(n) + 1.0f) / 2.0f;
}

vec3 CalculateNormalMap()
{
    vec3 BumpMapNormal = texture(NormalMap, TexCoords).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 NewNormal;

    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
    return NewNormal;
}


void main()
{    

	float clipDepth = gl_FragCoord.z;// - FragPos.z / FragPos.w;

	float dx = dFdx(clipDepth);
	float dy = dFdy(clipDepth);
	float moment2 = clipDepth * clipDepth + 0.25 * (dx * dx + dy * dy);

	gDepth = vec4(clipDepth, moment2, 0.0, 1.0);



    gNormal = vec4(PackNormal(CalculateNormalMap()), 1.0);
    //gNormal = vec4(PackNormal(Normal), 1.0);

    gAlbedo = texture(Texture, TexCoords);


	gSpecular = vec4(1);//texture(RoughnessMap, TexCoords);
;

}