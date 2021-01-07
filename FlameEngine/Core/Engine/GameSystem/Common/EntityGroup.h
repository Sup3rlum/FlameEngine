#pragma once


/*
*
*	Entity Groups provide a way of arranging scene enitities in a way that makes
*   dealing with them easier to deal with in batches.
* 
*	Enitity groups are used when large numbers of the same or similar entity types
*	behave in predefined or predictable ways. They provide an interface for parallel
*	computing entity data.
* 
*	Entities can belong to different entity groups BUT they cannot belong to multiple
*	enitity groups of the same functionality. This makes sure each entity has a scene 
*	function neccessary to step the scene, without implictely coding very possible
*	function withing the entity itself
* 
* 
*/


#include "EntityBase.h"

EXPORT(class, FSceneEntityGroup)
{

public:


};

