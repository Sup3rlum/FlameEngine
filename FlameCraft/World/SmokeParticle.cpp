#include "SmokeParticle.h"


SmokeParticle::SmokeParticle(const ParticleEmitter<SmokeParticle>& emitter) :
	Life(10.0f),
	Age(0)
{

	float angle = PI * ((float)(rand() % 360) / 180.0f);
	float dist = 30.0f;

	Position.xyz = emitter.Position;
	Position.x += cosf(angle);
	Position.z += sinf(angle);
	Velocity.xyz = 0;

}
