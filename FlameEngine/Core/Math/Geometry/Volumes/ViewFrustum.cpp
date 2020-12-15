#include "ViewFrustum.h"


ViewFrustum::ViewFrustum(FMatrix4 vp)
{
	viewProjection = vp;
}

void ViewFrustum::GetCorners(ViewFrustum vf, FVector3* corners)
{
    FVector4 ndc_corners[8] = 
    { 
        FVector4(-1.0f, -1.0f, 1.0f, 1.0f), 
        FVector4(-1.0f, -1.0f, -1.0f, 1.0f), 
        FVector4(-1.0f, 1.0f, 1.0f, 1.0f), 
        FVector4(-1.0f, 1.0f, -1.0f, 1.0f),
        FVector4(1.0f, -1.0f, 1.0f, 1.0f),
        FVector4(1.0f, -1.0f, -1.0f, 1.0f), 
        FVector4(1.0f, 1.0f, 1.0f, 1.0f), 
        FVector4(1.0f, 1.0f, -1.0f, 1.0f) 
    };
    FMatrix4 inverseVPMatrix = FMatrix4::Inverse(vf.viewProjection);


    for (uint32_t i = 0; i < 8; i++)
    {
        FVector4 p = inverseVPMatrix * ndc_corners[i];
        corners[i] = p.xyz / p.w;
    }
}
void ViewFrustum::GetPlanes(ViewFrustum vf, FPlane(&planes)[6])
{

}