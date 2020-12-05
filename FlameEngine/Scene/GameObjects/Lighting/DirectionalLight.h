#pragma once


#include "Light.h"


#define MAX_CASCADES


EXPORT(struct,  DirectionalLight) : public Light
{
public:
	fVector3 Direction;
	float Intensity;

	void SnapToFrustum(Camera* cam);
	void Update() override;
	void CreateFlare();
	void DrawFlare();

	DirectionalLight(fVector3 _direction, Color _color, float _intensity);

	fMatrix3 localSpace;
	AABB aabb;

private:
	float boundingIndex;
	fVector3 _positionInternal;

};