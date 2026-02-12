#version 450

out vec4 FragColor;

uniform vec3 LightDirection;
uniform float g = -0.99;
uniform float g2 = 0.99 * 0.99;

in vec3 v3Direction;

in vec3 fSecondaryColor;
in vec3 fColor;


float MieScatter(float fCos)
{
	return 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos*fCos) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);
}


void main ()
{
	float fCos = dot(-LightDirection, v3Direction);

	float fMiePhase = MieScatter(fCos);
	FragColor = vec4(fColor + fMiePhase * fSecondaryColor, 1.0f);
}