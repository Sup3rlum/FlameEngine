#pragma once

FORCEINLINE DXGI_FORMAT FRIAttributeFormatToDXGI(EFRIAttributeType Type)
{
	auto format = DXGI_FORMAT_UNKNOWN;
	switch (Type)
	{
		case EFRIAttributeType::Float:
			format = DXGI_FORMAT_R32_FLOAT;
			break;
		case EFRIAttributeType::Float2:
			format = DXGI_FORMAT_R32G32_FLOAT;
			break;
		case EFRIAttributeType::Float3:
			format = DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		case EFRIAttributeType::Float4:
			format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		case EFRIAttributeType::Int:
			format = DXGI_FORMAT_R32_SINT;
			break;
		case EFRIAttributeType::Int2:
			format = DXGI_FORMAT_R32G32_SINT;
			break;
		case EFRIAttributeType::Int3:
			format = DXGI_FORMAT_R32G32B32_SINT;
			break;
		case EFRIAttributeType::Int4:
			format = DXGI_FORMAT_R32G32B32A32_SINT;
			break;
		default:
			format = DXGI_FORMAT_UNKNOWN;
			break;
	}

	return format;
}
FORCEINLINE uint32 FRIAttributeFormatByteSize(EFRIAttributeType Type)
{
	uint32 ByteSize = 0;
	switch (Type)
	{
	case EFRIAttributeType::Float:
	case EFRIAttributeType::Int:
		ByteSize = 4;
		break;
	case EFRIAttributeType::Float2:
	case EFRIAttributeType::Int2:
		ByteSize = 8;
		break;
	case EFRIAttributeType::Float3:
	case EFRIAttributeType::Int3:
		ByteSize = 12;
		break;
	case EFRIAttributeType::Float4:
	case EFRIAttributeType::Int4:
		ByteSize = 16;
		break;
	default:
		return 0;
		break;
	}

	return ByteSize;
}

FORCEINLINE D3D12_INPUT_CLASSIFICATION FRIAttributeUsageFormatToClassification(EFRIAttribUsage Usage)
{
	auto format = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	if (Usage == EFRIAttribUsage::PerInstance) format = D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;
	return format;
}