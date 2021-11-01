#pragma once



struct IParticleBase
{
	float Age;
	float Life;

	IParticleBase(float Life) :
		Life(Life),
		Age(0)
	{}

};


struct IEmitterBase
{
	float EmitRate; // Particles per second
	bool Enabled;

	IEmitterBase(float EmitRate, bool enabled) :
		EmitRate(EmitRate),
		Enabled(enabled)
	{
	}

};

