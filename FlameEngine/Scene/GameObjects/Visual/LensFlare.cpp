#include "LensFlare.h"


LensFlare::LensFlare(Context* context)
{
	flares.push_back(LensFlareComponent(new Texture(" "), 1.0f));
	flares.push_back(LensFlareComponent(new Texture(" "), 1.0f));
	flares.push_back(LensFlareComponent(new Texture(" "), 1.0f));
	flares.push_back(LensFlareComponent(new Texture(" "), 1.0f));
	flares.push_back(LensFlareComponent(new Texture(" "), 1.0f));
}


void LensFlare::Render(Camera* cam)
{

	fMatrix4 vp = fMatrix4::Transpose(cam->View * cam->Projection);

	screenPosition = (vp * fVector4(-sunDirection, 0.0f)).xy;






}