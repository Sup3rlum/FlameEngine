#pragma once


#include "Core/Common/CoreCommon.h"

#include "EntityComponent/Entity.h"

class BehaviourScript
{
protected:
	virtual void Load() = 0;
	virtual void Update() = 0;

	friend class BehaviourSystem;
	friend class Behaviour;
	
	Entity& AttachedEntity()
	{
		return *pEntity;
	}
	const Entity& AttachedEntity() const
	{
		return AttachedEntity();
	}

private:
	Entity* pEntity;
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
		pScript->Load();
	}
private:
	BehaviourScript* pScript = NULL;
	Entity pEntity;

	friend class BehaviourSystem;
	friend class Scene;
};