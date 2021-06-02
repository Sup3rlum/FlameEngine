#pragma once

#pragma unmanaged

#include "Core/Engine/GameSystem/Game.h"
#include "FirstPersonComponent.h"

namespace FlameEditorCLR
{

	class EditorApplication : public GameApplication
	{
	public:
		EditorApplication(class Win32Context* context);
		void Update();
		void Load();

		Entity floorEntity;
		Entity boneGuy;


	};

}