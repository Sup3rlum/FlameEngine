#include "StaticModelMesh.h"
#include "../../../Scene/Scene.h"

void StaticModelMesh::Render(Scene* _scene, FMatrix4 _transform)
{
	
	pRenderingProgram->UseProgram();

	pRenderingProgram->SetUniform("View", _scene->CurrentCamera()->View);
	pRenderingProgram->SetUniform("Projection", _scene->CurrentCamera()->Projection);
	pRenderingProgram->SetUniform("World", _transform * LocalTransform);

	pRenderingProgram->SetUniform("CameraPosition", _scene->CurrentCamera()->Position);
	pRenderingProgram->SetUniform("LightDirection", _scene->DirectionalLightCollection[0].Direction);


	pRenderingProgram->SetUniform("Texture", 0);
	pRenderingProgram->SetUniform("NormalMap", 1);
	pRenderingProgram->SetUniform("RoughnessMap", 2);

	pRenderingProgram->SetTexture(0, mMeshMaterial->GetMap("Color"));
	pRenderingProgram->SetTexture(1, mMeshMaterial->GetMap("Normal"));
	pRenderingProgram->SetTexture(2, mMeshMaterial->GetMap("Roughness"));


	mVertexBuffer->RenderIndexed(GL_TRIANGLES);
}

void StaticModelMesh::Update()
{

}