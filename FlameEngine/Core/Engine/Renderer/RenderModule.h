#pragma once


#include "Core/Common/CoreCommon.h"
#include "Core/Engine/FlameRI/FRI.h"
#include "Core/Engine/FlameRI/ShaderLibrary/ShaderLibrary.h"
#include "../GameSystem/Common/Scene.h"

struct RStageInterface
{
	FStaticArray<FRIResourceObject*, 24> Buffer;

	template<typename TFRIResourceType>
	TFRIResourceType* GetResource(uint64 index)
	{
		return static_cast<TFRIResourceType*>(Buffer[index]);
	}

};