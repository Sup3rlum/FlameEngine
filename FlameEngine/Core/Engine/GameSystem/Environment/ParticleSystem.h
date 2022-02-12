#pragma once

#include "Core/Common/Alignment.h"
#include "Core/Math/Module.h"
#include "Core/Engine/FlameRI/FRI.h"


#include "ParticleInterface.h"


enum class PSComputeMode
{
	Software,
	GPUCompute
};

/*
struct IndexStack
{
	IndexStack(int32 capacity) :
		Data(NULL),
		Capacity(capacity),
		Top(0)
	{
		Data = Memory::AllocCounted<int32>(capacity);
		Memory::Zero(Data, sizeof(int32) * capacity);
	}

	void Push(int32 val)
	{
		if (Top < Capacity - 1)
		{
			Data[Top++] = val;
		}
	}
	int32 Pop()
	{
		if (Top > 0)
		{
			return Data[--Top];
		}
		else
		{
			return 0;
		}
	}

	~IndexStack()
	{
		Memory::Free(Data);
	}


	int32 Capacity;
	int32 Top;
	int32* Data;
};*/


template<typename TStage>
struct PStageBuffer
{
	PStageBuffer(int32 Capacity) :
		Size(0)
	{
		Data = Memory::AllocCounted<TStage>(Capacity);
		Memory::Zero(Data, Capacity * sizeof(TStage));
	}

	void Reset()
	{
		Size = 0;
	}

	void Add(const TStage& stage)
	{
		Data[Size++] = stage;
	}

	int32 ByteSize() const
	{
		return sizeof(TStage) * Size;
	}

	int32 Size;
	TStage* Data;
};


template<typename TParticle>
struct ParticleMemoryPool
{

	const int32 Capacity;
	const int32 AlignedSize;

	ParticleMemoryPool(size_t Capacity) :
		Data(NULL),
		Top(NULL),
		Capacity(Capacity),
		AlignedSize(sizeof(TParticle)),
		ParticleCount(0)
	{
		Data = Memory::AllocCounted<TParticle>(Capacity);
		Memory::Zero(Data, Capacity * AlignedSize);

		Top = Data;
	}

	TParticle* Allocate()
	{
		if (ParticleCount < Capacity)
		{
			ParticleCount++;
		}

		TParticle* AllocPtr = Top;
		TParticle* NewTop = AllocPtr + 1;

		if (NewTop >= Data + Capacity)
		{
			NewTop = Data;
		}

		Top = NewTop;

		return AllocPtr;
	}

	~ParticleMemoryPool()
	{
		Memory::Free(Data);
	}

	TParticle* Top;
	TParticle* Data;

	int32 ParticleCount;
};






class ParticleSystemBase
{
	PSComputeMode ComputeMode;
	bool Enabled;

	friend class Scene;
	friend class ParticleRenderer;

protected:
	ParticleSystemBase(PSComputeMode computeMode) : 
		ComputeMode(computeMode),
		Enabled(true)
	{
	}

	virtual FRIInstanceBuffer* GetInstanceBuffer() = 0;
	virtual int32 GetStageCount() = 0;

	virtual void Tick(float dt) = 0;
	virtual ~ParticleSystemBase()
	{}
};


template<typename TParticle, typename TStage, typename TEmitter>
class ParticleSystem : public ParticleSystemBase
{
	void Tick(float dt)
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

				TickDelegate(PMemory.Data[i], dt);
				StageBuffer.Add(STDelegate(PMemory.Data[i]));
			}
		}

		cmdList.GetDynamic()->InstanceBufferSubdata(InstanceBuffer, FRIUpdateDescriptor(StageBuffer.Data, 0, StageBuffer.ByteSize()));


		int32 EmitterNum = Emitters.Length();

		for (int i = 0; i < EmitterNum; i++)
		{
			if (Emitters[i].Enabled)
			{
				float EmitFreq = 1.0f / Emitters[i].EmitRate;

				EmitterAccum[i] += dt;
				if (EmitterAccum[i] >= EmitFreq)
				{
					EmitterAccum[i] -= EmitFreq;

					// Emit new particle

					TParticle* ParticleMem = PMemory.Allocate();
					new (ParticleMem) TParticle(Emitters[i]);
				}
			}
		}
	}


public:

	typedef FDelegate<TStage(TParticle&)> StageTransformDelegate;
	typedef FDelegate<void(TParticle&, float)> ParticleTickDelegate;

	ParticleSystem(FRIContext* FriContext, int32 Capacity) :
		ParticleSystemBase(PSComputeMode::Software),
		FriContext(FriContext),
		PMemory(Capacity),
		StageBuffer(Capacity)
	{
		SetStageTransform([](TParticle& p) { return TStage(); });
		SetParticleTick([](TParticle& p, float dt) {});

		FRICommandList cmdList(FriContext->GetFRIDynamic());

		InstanceBuffer = cmdList.GetDynamic()->CreateInstanceBuffer(Capacity, sizeof(TStage), FRICreationDescriptor(NULL, sizeof(TStage) * Capacity));
	}

	void SetStageTransform(StageTransformDelegate transformDelegate)
	{
		//assert(transformDelegate.IsValid());
		STDelegate = transformDelegate;
	}

	void SetParticleTick(ParticleTickDelegate tickDelegate)
	{
		//assert(tickDelegate.IsValid());
		TickDelegate = tickDelegate;
	}


	void AddEmitter(const TEmitter& emitter)
	{
		Emitters.Add(emitter);
		EmitterAccum.Add(0);
	}

	FRIInstanceBuffer* GetInstanceBuffer()
	{
		return InstanceBuffer;
	}

	int32 GetStageCount()
	{
		return StageBuffer.Size;
	}

	~ParticleSystem()
	{
		delete InstanceBuffer;
	}

	StageTransformDelegate STDelegate;
	ParticleTickDelegate TickDelegate;

	FRIInstanceBuffer* InstanceBuffer;
	FRIContext* FriContext;

	FArray<TEmitter> Emitters;
	FArray<float> EmitterAccum;
	ParticleMemoryPool<TParticle> PMemory;
	PStageBuffer<TStage> StageBuffer;

	friend class Scene;
};

