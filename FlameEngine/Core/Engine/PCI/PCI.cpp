#include "PCI.h"
#include "../FlameRI/FRI.h"


const size_t PropertyByteSize[] =
{
	4,//PropInt32,
	8,//PropInt64,
	4,//PropUint32,
	8,//PropUint64,
	4,//PropFloat,
	8,//PropVector2,
	12,//PropVector3,
	16,//PropVector4,
	4,//PropBool,
	-1,//PropString,
	4,//PropEnum,
	-1//PropNone
};

FORCEINLINE void GPUProperties::Serialize(FRIMemoryMap& GPUMemory)
{
	GPUMemory.Load(this->Start, this->TotalSize);
}