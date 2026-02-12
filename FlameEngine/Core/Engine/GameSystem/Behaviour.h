#pragma once


#include "Core/Common/CoreCommon.h"

#include "EntityComponent/Entity.h"

class Scene;
class GameApplication;

EXPORT(class, BehaviourScript)
{
protected:
	virtual void Load() = 0;
	virtual void Update(float dt) = 0;
	
	Scene& CurrentScene();
	const Scene& CurrentScene() const;

	Entity& AttachedEntity();
	const Entity& AttachedEntity() const;

	GameApplication* Game();

	template<typename TComponent>
	FORCEINLINE TComponent& Component()
	{
		assert(pEntity != nullptr);
		return pEntity->Component<TComponent>();
	}


	friend class Scene;
	friend class Behaviour;

private:
	Entity* pEntity;
	Scene* pScene;
	GameApplication* pGame;
};


class Behaviour
{

public:
	template<typename TBScript, typename ...TArgs>
	void AttachNew(TArgs... args)
	{
		//if (pScript)
			//delete pScript;

		pScript = new TBScript(args...);
		pScript->pEntity = &pEntity;
		pScript->pScene = pScene;
		pScript->pGame = pGame;
		pScript->Load();
	}

	template<typename TBScript>
	TBScript* ScriptAs()
	{
		return static_cast<TBScript*>(pScript);
	}

private:
	BehaviourScript* pScript = NULL;
	Entity pEntity;
	Scene* pScene;
	GameApplication* pGame;

	friend class Scene;
};