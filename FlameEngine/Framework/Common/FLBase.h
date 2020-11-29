#pragma once

#include "../../dll/framework.h"
#include "../../util/macros.h"

struct FLAME_DLL FLBase
{
public:
	virtual STRING ToString();
};


#define EXPORT_FORWARD_DECL(type, name) type FLAME_DLL name
//#define EXPORT(type, name, ...)	type FLAME_DLL name : FLBase, __VA_ARGS__
#define EXPORT(type, name)	type FLAME_DLL name 
#define EXPORT_ENUM	enum class FLAME_DLL
