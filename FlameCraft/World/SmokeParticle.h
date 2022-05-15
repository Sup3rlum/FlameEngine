#pragma once


#include <FlameEngine/Core/Engine/GameSystem/Environment/ParticleInterface.h>
#include <FlameEngine/Core/Math/Module.h>


struct SmokeParticle
{
	FVector4 Position;
	FVector4 Velocity;
	float Life;
	float Age;
	float _Padding[2];
	

	SmokeParticle(const ParticleEmitter<SmokeParticle>& emitter);
	constexpr static size_t GetStageSize()
	{
		return 48;
	}
};

template<>
struct ParticleEmitter<SmokeParticle> : IEmitterBase
{
	FVector3 Position;
	ParticleEmitter(FVector3 pos) : IEmitterBase(50.0f, true), Position(pos)
	{

	}
};