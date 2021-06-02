#include "FPXScene.h"


void FPXScene::Step(float dt)
{

	StepAccumulator += dt;
	if (StepAccumulator >= StepSize)
	{
		if (StepAccumulator >= 2 * StepSize)
		{
			StepAccumulator = 0;
		}


		StepAccumulator -= StepSize;

		sceneHandle->simulate(StepSize*2.0f);
		sceneHandle->fetchResults(true);

		//printf("Updating: Delta %f Accumulation %f\n", dt, StepAccumulator);
	}
	else
	{
		//printf("Accumulating: Delta %f Accumulation: %f\n", dt, StepAccumulator);
	}

	

}