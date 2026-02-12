#version 450 core

 out vec4 gDepth;


void main()
{    

	float clipDepth = gl_FragCoord.z;// - FragPos.z / FragPos.w;

	float dx = dFdx(clipDepth);
	float dy = dFdy(clipDepth);
	float moment2 = clipDepth * clipDepth + 0.25 * (dx * dx + dy * dy);

	gDepth = vec4(clipDepth, moment2, 0.0, 1.0);

}