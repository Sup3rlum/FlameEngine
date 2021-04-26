#version 450

out vec4 gDepth;

in vec4 FragPosLightSpace;


void main()
{
	float clipDepth = gl_FragCoord.z;// - FragPos.z / FragPos.w;
	//float clipDepth = abs(0.1f * FragPosLightSpace.z);// - FragPos.z / FragPos.w;

	//float clipDepth = length(FragPosLightSpace.xyz);
    
	/*float dx = dFdx(clipDepth);
	float dy = dFdy(clipDepth);*/
	float moment2 = clipDepth * clipDepth;// + 0.25f * (dx * dx + dy * dy);

	gDepth = vec4(clipDepth, moment2, 0.0, 1.0);
	//gDepth = DistributeFP(vec2(clipDepth, moment2));
}
