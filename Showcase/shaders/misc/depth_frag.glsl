#version 410

in vec4 screenSpacePosition;

out vec4 fragColor;



void main()
{             
	/*float ndcDepth = (2.0 * gl_FragCoord.z - gl_DepthRange.near - gl_DepthRange.far) / (gl_DepthRange.far - gl_DepthRange.near);

	float clipDepth = ndcDepth / gl_FragCoord.w;

	clipDepth = 0.5 * clipDepth + 0.5;
	*/

	float clipDepth = gl_FragCoord.z;

	float dx = dFdx(clipDepth);
	float dy = dFdy(clipDepth);
	float moment2 = clipDepth * clipDepth + 0.25 * (dx * dx + dy * dy);

	fragColor = vec4(clipDepth, moment2, 0.0, 1.0);
}  