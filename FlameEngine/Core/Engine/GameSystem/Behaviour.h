#pragma once


#include "Core/Common/CoreCommon.h"

#include "EntityComponent/Entity.h"

class Scene;

EXPORT(class, BehaviourScript)
{
protected:
	virtual void Load() = 0;
	virtual void Update(float dt) = 0;
	
	Scene& CurrentScene();
	const Scene& CurrentScene() const;

	Entity& AttachedEntity();
	const Entity& AttachedEntity() const;


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
};


class Behaviour
{

public:
	template<typename TBScript, typename ...TArgs>
	void Attach(TArgs... args)
	{
		//if (pScript)
			//delete pScript;

		pScript = new TBScript(args...);
		pScript->pEntity = &pEntity;
		pScript->pScene = pScene;
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

	friend class Scene;
};