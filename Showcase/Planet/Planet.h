#pragma once

#include <FlameEngine/Core/Engine/GameSystem/RenderObject.h>
#include "FlameEngine/Core/Engine/ContentSystem/ImportScripts/Common.h"
#include <FlameEngine/Core/Engine/ContentSystem/Client/LocalAssetManager.h>
#include <FlameEngine/Core/Engine/Renderer/Common/Geometry/VertexComponent.h>

struct Crater
{
	FVector3 Position;
	float Radius;
	float Depth;
	float rimWidth;
	float rimHeight;
};

struct NoiseParams
{
	float Frequency;
	float Octaves;
	float ScaleWidth;
	float ScaleHeight;
	float Persistence;
	float Lacunarity;
};


struct PlanetDesc
{
	FArray<Crater> Craters;

	NoiseParams ContinentNoise;
	NoiseParams OceanNoise;
	NoiseParams MountainNoiseMask;


	float Radius;
	float SurfaceGravity;

};



class Planet : public RenderList
{
public:
	Mesh mesh;
	Material material;
	FRIContext* FriContext;
	FRIUnorderedAccessView* MeshVertexUAV;
	FRIUnorderedAccessView* MeshIndexUAV;
	FRIUnorderedAccessView* IntermediateNormalUAV;
	FRIUnorderedAccessView* CraterBufferUAV;
	FRIComputeBuffer* IntermediateNormalBuffer;
	FRIComputeBuffer* CraterBuffer;

	FRIConstantBuffer* CraterParams;
	FRIConstantBuffer* ContinentNoiseParams;
	FRIConstantBuffer* OceanNoiseParams;
	FRIConstantBuffer* MountainMaskNoiseParams;

	FRIPipelineStateObject* PlanetRender;
	FRIPipelineStateObject* GenerationCompute;
	FRIPipelineStateObject* RenormalizeCompute;
	FRIPipelineStateObject* RenormalizeCompute2;

	PlanetDesc Desc;

	FRIPipelineStateObject* CreateGeometryPipelineState(const ShaderLibraryModule& Shaders);
	FRIPipelineStateObject* CreateComputePipeline(const ShaderLibraryModule& Shader, uint32 UAVCount, uint32 CBVCount);

	void SetOceanNoiseParams(
		float Frequency,
		float Octaves,
		float ScaleWidth,
		float ScaleHeight,
		float Persistence,
		float Lacunarity);

	void SetContinentNoiseParams(
		float Frequency,
		float Octaves,
		float ScaleWidth,
		float ScaleHeight,
		float Persistence,
		float Lacunarity);

	void SetMountainMaskNoiseParams(
		float Frequency,
		float Octaves,
		float ScaleWidth,
		float ScaleHeight,
		float Persistence,
		float Lacunarity);

	void SetCraterParams(int numCraters, float MinRadius, float MaxRadius, float Bias);

	void Generate();
	void GenerateMesh(const PlanetDesc& description);
	Planet(FRIContext* FriContext);
	void AddToCmdList(FRICommandList& cmdList, GRenderMode renderMode) override;
};