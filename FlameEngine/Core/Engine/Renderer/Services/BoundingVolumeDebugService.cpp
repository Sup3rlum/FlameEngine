#include "BoundingVolumeDebugService.h"

#include "Core/Engine/ContentSystem/Client/AssetImportScripts/ShaderLibrary.h"

BoundingVolumeDebugService::BoundingVolumeDebugService()

{



}

void BoundingVolumeDebugService::CreateResources(FRIContext* context)
{
	FRICommandList cmdList(context->GetFRIDynamic());

	Shaders = FLocalContent::LoadFromLocal<ShaderLibrary>("shaders/bbox_dx.fslib", context);

	pipeline = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["Debug"]);

	
	vBuffer = cmdList.GetDynamic()->CreateVertexBuffer(24, 1, FRICreationDescriptor(0, 24 * sizeof(FVertexComponent_Color)));


	FRIVertexDeclarationDesc vBufferDecl;
	vBufferDecl.InputSlot = 0;
	vBufferDecl.Components.Add(FRIVertexDeclarationComponent("POSITION", 3, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 24, 0));
	vBufferDecl.Components.Add(FRIVertexDeclarationComponent("COLOR", 3, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 24, 12));

	FArray<FRIVertexDeclarationDesc> VTemp;
	VTemp.Add(vBufferDecl);

	FRIVertexShader* signatureShader = cmdList.GetDynamic()->CreateVertexShader(Shaders.Modules["Debug"].Parts[EFRIResourceShaderType::Vertex].Memory);

	VertexDecl = cmdList.GetDynamic()->CreateVertexDeclaration(VTemp, signatureShader);
	cmdList.GetDynamic()->AttachVertexDeclaration(vBuffer, VertexDecl);

	delete signatureShader;
}

void BoundingVolumeDebugService::Render(FRICommandList& cmdList, FVector3* corners)
{
	FVertexComponent_Color vData[24] = 
	{
		FVertexComponent_Color(corners[0], Color::Blue.rgb),
		FVertexComponent_Color(corners[1], Color::Blue.rgb),

		FVertexComponent_Color(corners[1], Color::Blue.rgb),
		FVertexComponent_Color(corners[2], Color::Blue.rgb),
			
		FVertexComponent_Color(corners[2], Color::Blue.rgb),
		FVertexComponent_Color(corners[3], Color::Blue.rgb),
			  
		FVertexComponent_Color(corners[3], Color::Blue.rgb),
		FVertexComponent_Color(corners[0], Color::Blue.rgb),


		FVertexComponent_Color(corners[4], Color::Red.rgb),
		FVertexComponent_Color(corners[5], Color::Red.rgb),
			   
		FVertexComponent_Color(corners[5], Color::Red.rgb),
		FVertexComponent_Color(corners[6], Color::Red.rgb),
			   
		FVertexComponent_Color(corners[6], Color::Red.rgb),
		FVertexComponent_Color(corners[7], Color::Red.rgb),
			   
		FVertexComponent_Color(corners[7], Color::Red.rgb),
		FVertexComponent_Color(corners[4], Color::Red.rgb),


		FVertexComponent_Color(corners[0], Color::Green.rgb),
		FVertexComponent_Color(corners[4], Color::Green.rgb),

		FVertexComponent_Color(corners[1], Color::Green.rgb),
		FVertexComponent_Color(corners[5], Color::Green.rgb),

		FVertexComponent_Color(corners[2], Color::Green.rgb),
		FVertexComponent_Color(corners[6], Color::Green.rgb),

		FVertexComponent_Color(corners[3], Color::Green.rgb),
		FVertexComponent_Color(corners[7], Color::Green.rgb)
	};


	cmdList.GetDynamic()->VertexBufferSubdata(vBuffer, FRIUpdateDescriptor(vData, 0, sizeof(FVertexComponent_Color) * 24));

	
	cmdList.SetShaderPipeline(pipeline);


	cmdList.SetGeometrySource(vBuffer);
	cmdList.DrawPrimitives(EFRIPrimitiveType::Lines, 24);


}