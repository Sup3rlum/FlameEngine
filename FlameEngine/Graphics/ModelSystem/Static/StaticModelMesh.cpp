#include "StaticModelMesh.h"
#include "../../../Scene/Scene.h"

void StaticModelMesh::Render(Scene* _scene, fMatrix4 _transform)
{
	mShader->UseProgram();

	mShader->SetMatrix("View", _scene->CurrentCamera()->View);
	mShader->SetMatrix("Projection", _scene->CurrentCamera()->Projection);
	mShader->SetMatrix("World", _transform * LocalTransform);

	mShader->SetTexture(0, mMeshMaterial->mColorMap);


	mVertexBuffer->RenderIndexed(GL_TRIANGLES);
}

void StaticModelMesh::Update()
{

}