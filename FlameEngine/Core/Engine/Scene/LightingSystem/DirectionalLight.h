#pragma once


#include "Light.h"


#define MAX_CASCADES


EXPORT(struct,  DirectionalLight) : public Light
{
public:
	FVector3 Direction;
	float Intensity;

	void SnapToFrustum(Camera* cam);
	void Update() override;
	void CreateFlare();
	void DrawFlare();

	DirectionalLight(FVector3 _direction, Color _color, float _intensity);

	FMatrix3 localSpace;
	AABB aabb;

private:
	float boundingIndex;
	FVector3 _positionInternal;

};