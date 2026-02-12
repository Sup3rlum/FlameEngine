#version 450

layout(location = 0) out vec4 gDepth;


void main()
{
	float clipDepth = gl_FragCoord.z;

    
	/*float dx = dFdx(clipDepth);
	float dy = dFdy(clipDepth);*/
	float moment2 = clipDepth * clipDepth;// + 0.25f * (dx * dx + dy * dy);

	gDepth = vec4(clipDepth, moment2, 0.0, 1.0);
}
