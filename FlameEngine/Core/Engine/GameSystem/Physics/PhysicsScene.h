#pragma once


class PhysicsScene
{
public:
	virtual void Step(float dt) = 0;
	virtual void FetchResults() = 0;
};