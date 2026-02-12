#include "D3D12FRICommon.h"


void FD3D12Resource::TransitionState(D3D12_RESOURCE_STATES fromState, D3D12_RESOURCE_STATES toState)
{
	auto Parent = GetParentDevice();
	auto CommandList = Parent->GetCommandContext(0)->GetCommandListHandle();
	const auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(Resource.Get(), fromState, toState);
	CommandList->ResourceBarrier(1, &barrier);
	CurrentState = toState;
}

void FD3D12Resource::TransitionState(ID3D12GraphicsCommandList* CommandList, D3D12_RESOURCE_STATES After)
{
	if (CurrentState != After)
	{
		const auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(Resource.Get(), CurrentState, After);
		CommandList->ResourceBarrier(1, &barrier);
		CurrentState = After;
	}
}

void FD3D12TransientResource::UpdateSubresources(uint32 NumSubresources, const D3D12_SUBRESOURCE_DATA* Data)
{
	auto Parent = GetParentDevice();
	auto CommandList = Parent->GetCommandContext(0)->GetCommandListHandle();
	auto result = ::UpdateSubresources(CommandList.Get(), TargetResource->Resource.Get(), IntermediateResource->Resource.Get(), 0, 0, NumSubresources, Data);
}
void FD3D12TransientResource::UpdateSubresources(ID3D12GraphicsCommandList* CommandList, uint32 NumSubresources, const D3D12_SUBRESOURCE_DATA* Data)
{
	auto result = ::UpdateSubresources(CommandList, TargetResource->Resource.Get(), IntermediateResource->Resource.Get(), 0, 0, NumSubresources, Data);
}

void FD3D12TransientResource::RenameResources(const FString& Name)
{
	IntermediateResource->Resource->SetName(FString::Format(L"%0 (Upload)", Name).ToPlatformString());
	TargetResource->Resource->SetName(Name.ToPlatformString());
}