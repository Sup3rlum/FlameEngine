#pragma once

struct IEmitterBase
{
	float EmitRate; // Particles per second
	float Accumulator = 0;
	bool Enabled;

	IEmitterBase(float EmitRate, bool enabled) :
		EmitRate(EmitRate),
		Enabled(enabled)
	{
	}

};

template<typename TParticle>
struct ParticleEmitter;

#if __cplusplus >= 202002L
template<typename TParticle>
concept ParticleConcept = requires(TParticle p)
{
	{ p.Age };
	{ p.Life };
	{ TParticle::GetStageSize() };
	//{ ParticleEmitter<TParticle>{} };
};
#endif