#version 450

layout(location = 0) out vec4 FragColor;


layout(location = 1) in vec3 v3Direction;
layout(location = 2) in vec3 fSecondaryColor;
layout(location = 3) in vec3 fColor;


void main ()
{
	vec3 groundRay = vec3(v3Direction.x, 0, v3Direction.z);
	vec3 skyRay = v3Direction;
	skyRay.y *= 20.0f-5;

	float fCos = dot(normalize(skyRay), normalize(groundRay));

	vec3 finalColor;
	if (skyRay.y < 0)
	{
		finalColor = fSecondaryColor;
	}
	else
	{
		finalColor = mix(fColor, fSecondaryColor, fCos);
	}


	FragColor = vec4(finalColor, 1.0f);

	// Dark -> light

}