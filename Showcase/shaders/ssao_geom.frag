#version 450 core

layout (location = 0) out vec4 gDepth;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedo;


in vec2 TexCoords;
in vec4 FragPos;
in vec3 Normal;

uniform sampler2D Texture;


vec3 PackNormal(vec3 n)
{
	return (normalize(n) + 1.0f) / 2.0f;
}

void main()
{    

	float clipDepth = gl_FragCoord.z;// - FragPos.z / FragPos.w;

	float dx = dFdx(clipDepth);
	float dy = dFdy(clipDepth);
	float moment2 = clipDepth * clipDepth + 0.25 * (dx * dx + dy * dy);

	gDepth = vec4(clipDepth, moment2, 0.0, 1.0);



    gNormal = vec4(PackNormal(Normal), 1.0);

    gAlbedo = texture(Texture, TexCoords);

}