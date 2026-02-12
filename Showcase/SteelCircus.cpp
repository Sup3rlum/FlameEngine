#include "SteelCircus.h"
#include "GameScripts/PlayerScript.h"

#include "FlameEngine/Platform/Windows/Win32Context.h"
#include "FlameEngine/Core/Engine/ContentSystem/ImportScripts/Common.h"
#include "FlameEngine/Core/Engine/Renderer/Environment/SkySphere.h"


SteelCircus::SteelCircus(const FString& appName) : GameApplication(appName)
{
	srand(static_cast <unsigned> (time(0)));

}


void SteelCircus::Update(FGameTime gameTime)
{
	
}

void SteelCircus::Load()
{
	//gameScene = new GameScene(this, FriContext, PhysicsDescription());
	gameScene = new SpineScene(this, FriContext, PhysicsDescription());
	TransitionToScene(gameScene);

}
void SteelCircus::Suspend()
{


}

void SteelCircus::Dispose()
{


}

void SteelCircus::Close()
{
	FriContext->PollCloseEvent();
}