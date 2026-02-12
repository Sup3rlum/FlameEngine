#pragma once

#include "Core/Engine/GameSystem/Behaviour.h"
#include "Core/Math/Module.h"
#include "Core/Engine/GameSystem/InputComponent.h"
#include "Core/Engine/GameSystem/Material/MaterialComponent.h"
#include "Core/Engine/GameSystem/Mesh.h"

namespace FlameEditorCLR
{
	class EditorEngineApp;
}
class FRIContext;


struct FPComponent
{
	FVector3 Up;
	FVector3 Right;
	FVector3 LookDirection;
	float flySpeed = 100.0f;
	float pitch;
	float yaw = 2.3f;
	bool isCursorLocked;
};

class EditorViewerScript : public BehaviourScript
{
public:

	EditorViewerScript(FRIContext* FriContext, FlameEditorCLR::EditorEngineApp* app) :
		FriContext(FriContext),
		app(app)
	{}

	void Load();
	void Update(float delta);
	void UpdateMovement(float delta);

	void KeyEvent(FKeyboardKeys key, FKeyEvent event);
	void MouseEvent(FMouseButton key, FKeyEvent event);

	FRIContext* FriContext;
	FlameEditorCLR::EditorEngineApp* app;

};