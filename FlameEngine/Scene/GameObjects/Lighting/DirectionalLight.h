#pragma once


#include "Light.h"

EXPORT(struct,  DirectionalLight) : public Light
{
public:
	fVector3 Direction;
	float Intensity;

	void SnapToFrustum(Camera* cam);
	void Update() override;


	DirectionalLight(fVector3 _direction, Color _color, float _intensity);

private:
	float boundingIndex;
	fVector3 _positionInternal;
};