#version 450

out vec4 FragColor;


in vec3 v3Direction;

in vec3 fSecondaryColor;
in vec3 fColor;


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