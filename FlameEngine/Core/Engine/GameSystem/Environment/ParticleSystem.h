#pragma once

#include "Core/Common/Alignment.h"
#include "Core/Math/Module.h"
#include "Core/Engine/FlameRI/FRI.h"
#include "../Mesh.h"

#include "../CameraSystem/CameraComponent.h"

#include "ParticleInterface.h"


enum class PSComputeMode
{
	Software,
	GPUCompute
};


struct DeadList
{
	DeadList(size_t capacity) : 
		Capacity(capacity), 
		Count(capacity)
	{
		auto dataPtr = Memory::AllocCounted<int32>(Capacity + 1);
		Memory::Zero(dataPtr, (Capacity + 1) * sizeof(int32));
		Data = Align(dataPtr, alignof(int32));

		for (int i = 0; i < Capacity; i++)
		{
			Data[i] = Capacity - 1 - i;
		}
	}

	void Push(int32 index)
	{
		if (Count >= Capacity)
		{
			return;
		}
		Data[Count] = index;
		Count++;
	}

	int32 Pop()
	{
		if (Count <= 0)
		{
			return -1;
		}
		else
		{
			int32 LastIndex = Data[Count - 1];
			Count--;
			return LastIndex;
		}
	}


	int32* Data;
	size_t Count;
	size_t Capacity;
};

template<typename TParticle>
#if __cplusplus >= 202002L
requires ParticleConcept<TParticle>
#endif
struct ParticleMemoryPool
{
	ParticleMemoryPool(size_t Capacity) :
		Data(NULL),
		Capacity(Capacity),
		AlignedSize(sizeof(TParticle)),
		ParticleCount(0),
		deadList(Capacity)
	{
		Data = Memory::AllocCounted<TParticle>(Capacity);
		Memory::Zero(Data, Capacity * AlignedSize);
	}

	TParticle* Allocate()
	{
		int32 deadIndex = deadList.Pop();
		if (deadIndex < 0) return NULL;

		ParticleCount++;

		printf("Allocating index %d\n", deadIndex);

		auto AllocPtr = &Data[deadIndex];
		return AllocPtr;
	}


	void Deallocate(int32 index)
	{
		deadList.Push(index);
		ParticleCount--;

		Memory::Zero(Data + index, sizeof(TParticle));
	}

	~ParticleMemoryPool()
	{
		Memory::Free(Data);
	}

	DeadList deadList;

	const int32 Capacity;
	const int32 AlignedSize;

	TParticle* Data;
	int32 ParticleCount;
};




class IParticleSystem
{
	PSComputeMode ComputeMode;
	bool Enabled;

	friend class Scene;
	friend class ParticleRenderer;

protected:
	IParticleSystem(PSComputeMode computeMode) : 
		ComputeMode(computeMode),
		Enabled(true)
	{
	}

	virtual FRIInstanceBuffer* GetInstanceBuffer() = 0;
	virtual int32 GetStageCount() = 0;

	virtual void Tick(float dt, const Camera& cam) = 0;
	virtual ~IParticleSystem()
	{}

public:
	FArray<FRISampler> Samplers;
	Mesh ParticleMesh;
	FRIShaderPipeline* Shader;

};


template<typename TParticle>
#if __cplusplus >= 202002L
	requires ParticleConcept<TParticle>
#endif
class ParticleSystem : public IParticleSystem
{
	void Tick(float dt, const Camera& cam)
	{
		if (dt > 5.0f)
		{
			return;
		}

		FRICommandList cmdList(FriContext->GetFRIDynamic());

		for (int i = 0; i < PMemory.Capacity; i++)
		{
			if (PMemory.Data[i].Life > 0)
			{
				if (PMemory.Data[i].Age < PMemory.Data[i].Life)
				{
					PMemory.Data[i].Age += dt;
					TickDelegate(PMemory.Data[i], dt);
				}
				else
				{
					PMemory.Deallocate(i);
					printf("deleting particle index %d\n", i);

				}
			}
		}


		Sort::Insertion(PMemory.Data, PMemory.Capacity, [&](TParticle& particle) 
			{
				if (particle.Life == 0)
				{
					return -100000000.0f;
				}
				auto ViewDist = FMatrix4::Transpose(cam.View) * FVector4(particle.Position.xyz, 1.0f);
				//ViewDist = cam.Projection * ViewDist;
				return -ViewDist.z;
			});

		cmdList.GetDynamic()->InstanceBufferSubdata(InstanceBuffer, FRIUpdateDescriptor(PMemory.Data, 0, PMemory.Capacity * sizeof(TParticle)));

		for (auto& emitter : Emitters)
		{
			if (emitter.Enabled)
			{
				float EmitFreq = 1.0f / emitter.EmitRate;

				emitter.Accumulator += dt;
				if (emitter.Accumulator >= EmitFreq)
				{
					emitter.Accumulator -= EmitFreq;

					printf("Trying to emit\n");

					// Emit new particle
					auto ParticleMem = PMemory.Allocate();
					if (ParticleMem) 
					{
						printf("emitting particle\n");
						new (ParticleMem) TParticle(emitter);
					}
				}
			}
		}
	}


public:

	typedef FDelegate<void(TParticle&, float)> ParticleTickDelegate;

	ParticleSystem(FRIContext* FriContext, int32 Capacity) :
		IParticleSystem(PSComputeMode::Software),
		FriContext(FriContext),
		PMemory(Capacity)
	{
		SetParticleTick([](TParticle& p, float dt) {});

		FRICommandList cmdList(FriContext->GetFRIDynamic());
		InstanceBuffer = cmdList.GetDynamic()->CreateInstanceBuffer(Capacity, TParticle::GetStageSize(), FRICreationDescriptor(NULL, TParticle::GetStageSize() * Capacity));
	}

	void SetParticleTick(ParticleTickDelegate tickDelegate)
	{
		//assert(tickDelegate.IsValid());
		TickDelegate = tickDelegate;
	}


	void AddEmitter(const ParticleEmitter<TParticle>& emitter)
	{
		Emitters.Add(emitter);
	}

	FRIInstanceBuffer* GetInstanceBuffer()
	{
		return InstanceBuffer;
	}

	int32 GetStageCount()
	{
		return PMemory.Capacity;
	}

	~ParticleSystem()
	{
		delete InstanceBuffer;
	}

	FArray<ParticleEmitter<TParticle>> Emitters;
	ParticleMemoryPool<TParticle> PMemory;

	ParticleTickDelegate TickDelegate;

	FRIInstanceBuffer* InstanceBuffer;
	FRIContext* FriContext;

	friend class Scene;
};

/*
template<typename TParticle>
#if __cplusplus >= 202002L
	requires ParticleConcept<TParticle>
#endif
class GPUParticleSystem : public IParticleSystem
{
	void Tick(float dt, const Camera& cam)
	{
		if (dt > 5.0f)
		{
			return;
		}

		FRICommandList cmdList(FriContext->GetFRIDynamic());

		StageBuffer.Reset();

		for (int i = 0; i < PMemory.ParticleCount; i++)
		{
			if (PMemory.Data[i].Age < PMemory.Data[i].Life)
			{
				PMemory.Data[i].Age += dt;

				STDelegate(StageBuffer, PMemory.Data[i]);
			}
		}

		SortDelegate(StageBuffer, cam);

		cmdList.GetDynamic()->InstanceBufferSubdata(InstanceBuffer, FRIUpdateDescriptor(StageBuffer.Data, 0, StageBuffer.ByteSize()));

		for (auto& emitter : Emitters)
		{
			if (emitter.Enabled)
			{
				float EmitFreq = 1.0f / emitter.EmitRate;

				emitter.Accumulator += dt;
				if (emitter.Accumulator >= EmitFreq)
				{
					emitter.Accumulator -= EmitFreq;

					// Emit new particle

					TParticle* ParticleMem = PMemory.Allocate();
					new (ParticleMem) TParticle(emitter);
				}
			}
		}
	}


public:


	GPUParticleSystem(FRIContext* FriContext, int32 Capacity) :
		IParticleSystem(PSComputeMode::GPUCompute),
		FriContext(FriContext),
		PMemory(Capacity),
		StageBuffer(Capacity, TParticle::GetStageSize())
	{
		SetStageTransform([](PStageBuffer& map, TParticle& p) {});
		SetParticleTick([](TParticle& p, float dt) {});

		FRICommandList cmdList(FriContext->GetFRIDynamic());

		InstanceBuffer = cmdList.GetDynamic()->CreateInstanceBuffer(Capacity, TParticle::GetStageSize(), FRICreationDescriptor(NULL, TParticle::GetStageSize() * Capacity));
	}

	void AddEmitter(const ParticleEmitter<TParticle>& emitter)
	{
		Emitters.Add(emitter);
	}

	FRIInstanceBuffer* GetInstanceBuffer()
	{
		return InstanceBuffer;
	}

	int32 GetStageCount()
	{
		return StageBuffer.Size;
	}

	~GPUParticleSystem()
	{
		delete InstanceBuffer;
	}

	FArray<ParticleEmitter<TParticle>> Emitters;
	ParticleMemoryPool<TParticle> PMemory;

	FRIInstanceBuffer* InstanceBuffer;
	FRIContext* FriContext;

	friend class Scene;
};
*/

struct ParticleManager
{
	IParticleSystem* ParticleSystemPtr;
};