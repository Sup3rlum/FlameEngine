#pragma once

#include "Core/Common/CoreCommon.h"



bool OpenGLVer1ExtensionSupport()
{
	return true;
}

bool OpenGLVer2ExtensionSupport()
{
	return true;
}

bool OpenGLVer3ExtensionSupport()
{
	return true;
}

bool OpenGLVer4ExtensionSupport()
{
	return true;
}

bool OpenGLVer45ExtensionSupport()
{
	return true;
}

bool OpenGLVerRTSupport()
{
	return true;
}

bool OpenGLVerCompatibility()
{
	return true;
}

typedef bool (*ExtensionSupportPolicyFunc)();

struct OpenGLContextDrvVersion
{
	virtual uint32 GetMajor() { return 0; }
	virtual uint32 GetMinor() { return 0; }
	virtual bool IsLegacy() { return true; }

	virtual bool CheckIsSupportedOnPlatform() { return false; }


};


template<uint32 Major, uint32 Minor, bool isLegacy, ExtensionSupportPolicyFunc... extFunc>
struct TOpenGLContextDrvVersion : public OpenGLContextDrvVersion
{
	uint32 GetMajor() const { return Major; }
	uint32 GetMinor() const { return Minor; }
	bool IsLegacy() const { return isLegacy;  }


	bool CheckIsSupportedOnPlatform() { return ((extfunc)() && ...) }
};


typedef TOpenGLContextDrvVersion<1, 0, true, OpenGLVer1ExtensionSupport>							FOpenGLVersion_10;
typedef TOpenGLContextDrvVersion<1, 1, true, OpenGLVer1ExtensionSupport>							FOpenGLVersion_11;
typedef TOpenGLContextDrvVersion<1, 2, true, OpenGLVer1ExtensionSupport>							FOpenGLVersion_12;
typedef TOpenGLContextDrvVersion<1, 3, true, OpenGLVer1ExtensionSupport>							FOpenGLVersion_13;
typedef TOpenGLContextDrvVersion<1, 4, true, OpenGLVer1ExtensionSupport>							FOpenGLVersion_14;
typedef TOpenGLContextDrvVersion<1, 5, true, OpenGLVer1ExtensionSupport>							FOpenGLVersion_15;
		
typedef TOpenGLContextDrvVersion<3, 0, false, OpenGLVer3ExtensionSupport, OpenGLVerCompatibility>	FOpenGLVersion_30;
typedef TOpenGLContextDrvVersion<3, 1, false, OpenGLVer3ExtensionSupport, OpenGLVerCompatibility>	FOpenGLVersion_31;
typedef TOpenGLContextDrvVersion<3, 2, false, OpenGLVer3ExtensionSupport, OpenGLVerCompatibility>	FOpenGLVersion_32;
typedef TOpenGLContextDrvVersion<3, 3, false, OpenGLVer3ExtensionSupport, OpenGLVerCompatibility>	FOpenGLVersion_33;
		
typedef TOpenGLContextDrvVersion<4, 0, false, OpenGLVer4ExtensionSupport>							FOpenGLVersion_40;
typedef TOpenGLContextDrvVersion<4, 1, false, OpenGLVer4ExtensionSupport>							FOpenGLVersion_41;
typedef TOpenGLContextDrvVersion<4, 2, false, OpenGLVer4ExtensionSupport>							FOpenGLVersion_42;
typedef TOpenGLContextDrvVersion<4, 3, false, OpenGLVer4ExtensionSupport>							FOpenGLVersion_43;
typedef TOpenGLContextDrvVersion<4, 4, false, OpenGLVer4ExtensionSupport>							FOpenGLVersion_44;
typedef TOpenGLContextDrvVersion<4, 5, false, OpenGLVer45ExtensionSupport, OpenGLVerRTSupport>		FOpenGLVersion_45;
typedef TOpenGLContextDrvVersion<4, 6, false, OpenGLVer45ExtensionSupport, OpenGLVerRTSupport>		FOpenGLVersion_46;
