#pragma once

#include "FlameEngine/Core/Engine/GameSystem/Environment/ParticleSystem.h"




struct TestEmitter : IEmitterBase
{
	FVector3 Position;

	TestEmitter(FVector3 pos) : IEmitterBase(50, true), Position(pos)
	{}
};

struct TestParticle : IParticleBase
{
	FVector3 Position;
	FVector3 Velocity;
	Color Color;

	TestParticle(TestEmitter& emitter) : 
		IParticleBase(2),
		Position(emitter.Position),
		Color(Color::Yellow)
	{


		float vX = (float)(rand() % 100) / 50.0f - 1.0f;
		float vZ = (float)(rand() % 100) / 50.0f - 1.0f;

		Velocity = FVector3(vX*3, 20.0f, vZ*3);
	}
};


struct TestParticleStage
{
	FMatrix4 Translation;
	Color color;

	TestParticleStage()
	{}

	TestParticleStage(const FMatrix4& matrix, Color color) :
		Translation(matrix),
		color(color)
	{

	}
};

typedef ParticleSystem<TestParticle, TestParticleStage, TestEmitter> TestParticleSystem;