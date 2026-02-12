#include "ViewSpaceDecal.h"

void ViewSpaceDecal::AddToRenderList(FRICommandList& cmdList) const
{
	mesh.AddToRenderList(cmdList, EFRIPrimitiveType::Triangles);
}
