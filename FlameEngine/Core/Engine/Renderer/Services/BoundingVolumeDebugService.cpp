#include "BoundingVolumeDebugService.h"


BoundingVolumeDebugService::BoundingVolumeDebugService(FRICommandList& cmdList) :
	vDecl(FArray<FResourceVertexDeclarationComponent>())
{

	FAnsiString vsData = IOFileStream("./shaders/debug_view.vert").ReadAnsiFile();
	FAnsiString psData = IOFileStream("./shaders/debug_view.frag").ReadAnsiFile();

	FResourceVertexShader* vs = cmdList.GetDynamic()->DynamicCreateVertexShader(vsData);
	FResourcePixelShader* ps = cmdList.GetDynamic()->DynamicCreatePixelShader(psData);

	pipeline = cmdList.GetDynamic()->DynamicCreateShaderPipeline
	(
		FResourceShaderPipelineCreationDescriptor(
			2,
			new FResourceShaderBase * [2]{ vs, ps }
		)
	);

	FArray<FResourceVertexDeclarationComponent> vArray =
	{
		FResourceVertexDeclarationComponent(0, 3, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 24, 0),
		FResourceVertexDeclarationComponent(1, 3, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 24, 12)
	};



	ViewLoc = cmdList.GetDynamic()->GetShaderUniformParameterLocation(pipeline, "View");
	ProjLoc = cmdList.GetDynamic()->GetShaderUniformParameterLocation(pipeline, "Projection");

	vDecl.DeclarationElements = vArray;


	std::cout << sizeof(FVertexComponent_Color) << std::endl;

}

void BoundingVolumeDebugService::Render(FRICommandList& cmdList, const CameraComponent& cam, FVector3* corners)
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



	vBuffer = cmdList.GetDynamic()->DynamicCreateVertexBuffer(24, 0, FResourceCreationDescriptor(vData, 24 * sizeof(FVertexComponent_Color)));

	cmdList.GetDynamic()->AttachVertexDeclaration(vDecl);

	cmdList.SetShaderPipeline(pipeline);

	cmdList.SetShaderUniformParameter(FUniformParameter(ViewLoc, cam.View));
	cmdList.SetShaderUniformParameter(FUniformParameter(ProjLoc, cam.Projection));

	cmdList.SetGeometrySource(vBuffer);
	cmdList.DrawPrimitives((uint32)EFRIPrimitiveType::Lines, 24);


}