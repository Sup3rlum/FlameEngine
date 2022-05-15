#include "DebugRenderer.h"

#include "Core/Engine/ContentSystem/ImportScripts/ShaderLibrary.h"
#include "Core/Engine/ContentSystem/Client/LocalAssetManager.h"

DebugRenderer::DebugRenderer()

{



}

void DebugRenderer::CreateResources(FRIContext* context)
{
	FRICommandList cmdList(context->GetFRIDynamic());

	FAssetManager Content;
	Content.Connect("./Assets/");
	Content.RenderContext = context;
	Shaders = Content.Load<ShaderLibrary>("Shaders/bbox_dx.fslib");

	pipeline = cmdList.GetDynamic()->CreateShaderPipeline(Shaders.Modules["Debug"]);
	vBuffer = cmdList.GetDynamic()->CreateVertexBuffer(24, 1, FRICreationDescriptor(0, 24 * sizeof(FVertex_PositionColor)));

	FRIInputDesc vBufferDecl;
	vBufferDecl.InputSlot = 0;
	vBufferDecl.Components.Add(FRIInputAttribute("POSITION", 3, EFRIAttributeType::Float, EFRIBool::False, 24, 0));
	vBufferDecl.Components.Add(FRIInputAttribute("COLOR", 3, EFRIAttributeType::Float, EFRIBool::False, 24, 12));


	FRIVertexShader* signatureShader = cmdList.GetDynamic()->CreateVertexShader(Shaders.Modules["Debug"].Parts[EFRIShaderType::Vertex].Memory);

	VertexDecl = cmdList.GetDynamic()->CreateVertexDeclaration({ vBufferDecl }, signatureShader);
	cmdList.GetDynamic()->AttachVertexDeclaration(vBuffer, VertexDecl);

	delete signatureShader;
}

void DebugRenderer::RecreateResources(FRIContext* context, FRIContext* context2)
{
}

void DebugRenderer::Render(FRICommandList& cmdList, FVector3* corners)
{
	FVertex_PositionColor vData[24] = 
	{
		FVertex_PositionColor(corners[0], Color::Blue.rgb),
		FVertex_PositionColor(corners[1], Color::Blue.rgb),
				
		FVertex_PositionColor(corners[1], Color::Blue.rgb),
		FVertex_PositionColor(corners[2], Color::Blue.rgb),
				
		FVertex_PositionColor(corners[2], Color::Blue.rgb),
		FVertex_PositionColor(corners[3], Color::Blue.rgb),
			  	
		FVertex_PositionColor(corners[3], Color::Blue.rgb),
		FVertex_PositionColor(corners[0], Color::Blue.rgb),
				
		FVertex_PositionColor(corners[4], Color::Red.rgb),
		FVertex_PositionColor(corners[5], Color::Red.rgb),
			  
		FVertex_PositionColor(corners[5], Color::Red.rgb),
		FVertex_PositionColor(corners[6], Color::Red.rgb),
			   	
		FVertex_PositionColor(corners[6], Color::Red.rgb),
		FVertex_PositionColor(corners[7], Color::Red.rgb),
			   	
		FVertex_PositionColor(corners[7], Color::Red.rgb),
		FVertex_PositionColor(corners[4], Color::Red.rgb),
				
		FVertex_PositionColor(corners[0], Color::Green.rgb),
		FVertex_PositionColor(corners[4], Color::Green.rgb),
				
		FVertex_PositionColor(corners[1], Color::Green.rgb),
		FVertex_PositionColor(corners[5], Color::Green.rgb),
				
		FVertex_PositionColor(corners[2], Color::Green.rgb),
		FVertex_PositionColor(corners[6], Color::Green.rgb),
				
		FVertex_PositionColor(corners[3], Color::Green.rgb),
		FVertex_PositionColor(corners[7], Color::Green.rgb)
	};


	cmdList.GetDynamic()->VertexBufferSubdata(vBuffer, FRIUpdateDescriptor(vData, 0, sizeof(FVertex_PositionColor) * 24));

	cmdList.SetShaderPipeline(pipeline);

	cmdList.SetGeometrySource(vBuffer);
	cmdList.DrawPrimitives(EFRIPrimitiveType::Lines, 24);
}