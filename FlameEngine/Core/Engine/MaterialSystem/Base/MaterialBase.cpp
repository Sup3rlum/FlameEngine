#include "MaterialBase.h"


MaterialBase::MaterialBase()
{

}

void MaterialBase::SetMap(STRING name, Texture* map)
{
	if (map == NULL)
		return;

	_internal[name] = map;
}
Texture*  MaterialBase::GetMap(STRING name)
{
	return _internal[name];
}