#include "FlameCraft.h"
#include "PlayerScript.h"
#include "Renderer/WorldRenderer.h"

#include "FlameEngine/Platform/Windows/Win32Context.h"
#include "FlameEngine/Core/Engine/ContentSystem/ImportScripts/Common.h"

#include <FlameEngine/Core/Engine/GameSystem/Environment/ParticleSystem.h>
#include "World/SmokeParticle.h"
#include "Scenes/GameScene.h"

#include <FlameEngine/Core/Math/Calc/VectorField.h>

FlameCraft::FlameCraft(const FString& appName) : GameApplication(appName)
{
	srand(static_cast <unsigned> (time(0)));

}


void FlameCraft::Update(FGameTime gameTime)
{

}


void FlameCraft::Load()
{
	auto scene = new GameScene(this, FriContext, PhysicsDescription());
	TransitionToScene(scene);
}
void FlameCraft::Suspend()
{


}

void FlameCraft::Dispose()
{


}

void FlameCraft::Close()
{
	FriContext->PollCloseEvent();
}

void FlameCraft::CreateParticleSystem()
{

}