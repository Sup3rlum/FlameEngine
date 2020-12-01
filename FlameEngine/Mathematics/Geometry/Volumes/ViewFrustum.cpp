#include "ViewFrustum.h"


ViewFrustum::ViewFrustum(fMatrix4 vp)
{
	viewProjection = vp;
}

void ViewFrustum::GetCorners(ViewFrustum vf, fVector3* corners)
{
    fVector4 ndc_corners[8] = 
    { 
        fVector4(-1.0f, -1.0f, 1.0f, 1.0f), 
        fVector4(-1.0f, -1.0f, -1.0f, 1.0f), 
        fVector4(-1.0f, 1.0f, 1.0f, 1.0f), 
        fVector4(-1.0f, 1.0f, -1.0f, 1.0f),
        fVector4(1.0f, -1.0f, 1.0f, 1.0f),
        fVector4(1.0f, -1.0f, -1.0f, 1.0f), 
        fVector4(1.0f, 1.0f, 1.0f, 1.0f), 
        fVector4(1.0f, 1.0f, -1.0f, 1.0f) 
    };
    fMatrix4 inverseVPMatrix = fMatrix4::Inverse(vf.viewProjection);


    for (uint32_t i = 0; i < 8; i++)
    {
        fVector4 p = inverseVPMatrix * ndc_corners[i];
        corners[i] = p.xyz / p.w;
    }
}
void ViewFrustum::GetPlanes(ViewFrustum vf, fPlane(&planes)[6])
{

}