#include "Game.h"

#include "Core/Engine/FlameRI/OpenGL/OpenGLFRI.h"


GameApplication::GameApplication(const FString& Name) :
	applicationName(Name)
{

	FRIRenderingContextDescription desc;

	desc.Width = 800;
	desc.Width = 600;
	desc.IsFullscreen = false;
	desc.SampleCount = 0;

	FriContext = new OpenGLFRIContext(desc);
	FriContext->Initialize();

}

void GameApplication::Run()
{
	while (FriContext->HandleEvents())
	{



		FriContext->SwapBuffers();
	}
}

void GameApplication::InputHandlerFunc(FKeyboardKeys key, FKeyboardKeyEvent keyEvent)
{
	for (int i = 0; i < currentScene->controllableObjectReferences.Length(); i++)
	{

		auto controlRef = currentScene->controllableObjectReferences[i];

		if (controlRef)
		{
			for (int j = 0; j < controlRef->ControlComponent.KeyEventBindings.Length(); j++)
			{
				controlRef->ControlComponent.KeyEventBindings[i](key, keyEvent);
			}
		}
	}
}