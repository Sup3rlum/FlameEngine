#include "StaticModelMesh.h"
#include "../../../Scene/Scene.h"

void StaticModelMesh::Render(Scene* _scene, FMatrix4 _transform)
{
	
	pRenderingProgram->UseProgram();

	pRenderingProgram->SetUniform("View", _scene->CurrentCamera()->View);
	pRenderingProgram->SetUniform("Projection", _scene->CurrentCamera()->Projection);
	pRenderingProgram->SetUniform("World", _transform * LocalTransform);

	pRenderingProgram->SetTexture(0, mMeshMaterial->mColorMap);


	mVertexBuffer->RenderIndexed(GL_TRIANGLES);
}

void StaticModelMesh::Update()
{

}