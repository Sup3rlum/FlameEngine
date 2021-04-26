#pragma once

#include "Core/Common/CoreCommon.h"


struct OpenGLContextDrvVersion
{
	uint32 GetMajor() { return Major; }
	uint32 GetMinor() { return Minor; }
	bool IsLegacy() { return isLegacy; }

	bool CheckIsSupportedOnPlatform() { return true; }

protected:

	OpenGLContextDrvVersion(uint32 maj, uint32 min, bool leg) : Major(maj), Minor(min), isLegacy(leg) {}

	uint32 Major;
	uint32 Minor;
	bool isLegacy;

};


template<uint32 TMajor, uint32 TMinor, bool TisLegacy>
struct TOpenGLContextDrvVersion : public OpenGLContextDrvVersion
{
	
	TOpenGLContextDrvVersion() : OpenGLContextDrvVersion(TMajor, TMinor, TisLegacy) {}
};


typedef TOpenGLContextDrvVersion<1, 0, true>	FOpenGLVersion_10;
typedef TOpenGLContextDrvVersion<1, 1, true>	FOpenGLVersion_11;
typedef TOpenGLContextDrvVersion<1, 2, true>	FOpenGLVersion_12;
typedef TOpenGLContextDrvVersion<1, 3, true>	FOpenGLVersion_13;
typedef TOpenGLContextDrvVersion<1, 4, true>	FOpenGLVersion_14;
typedef TOpenGLContextDrvVersion<1, 5, true>	FOpenGLVersion_15;
		
typedef TOpenGLContextDrvVersion<3, 0, false>	FOpenGLVersion_30;
typedef TOpenGLContextDrvVersion<3, 1, false>	FOpenGLVersion_31;
typedef TOpenGLContextDrvVersion<3, 2, false>	FOpenGLVersion_32;
typedef TOpenGLContextDrvVersion<3, 3, false>	FOpenGLVersion_33;
		
typedef TOpenGLContextDrvVersion<4, 0, false>	FOpenGLVersion_40;
typedef TOpenGLContextDrvVersion<4, 1, false>	FOpenGLVersion_41;
typedef TOpenGLContextDrvVersion<4, 2, false>	FOpenGLVersion_42;
typedef TOpenGLContextDrvVersion<4, 3, false>	FOpenGLVersion_43;
typedef TOpenGLContextDrvVersion<4, 4, false>	FOpenGLVersion_44;
typedef TOpenGLContextDrvVersion<4, 5, false>	FOpenGLVersion_45;
typedef TOpenGLContextDrvVersion<4, 6, false>	FOpenGLVersion_46;
