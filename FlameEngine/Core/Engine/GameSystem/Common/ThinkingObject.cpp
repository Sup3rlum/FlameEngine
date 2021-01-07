#include "ThinkingObject.h"


void ThinkingObject::Think()
{
	LastThink = FTimeStamp::MarkCurrent();
}