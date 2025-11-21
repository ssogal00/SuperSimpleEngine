#pragma once

#include "SSVertexTypes.h"
#include <vector>
#include "SSName.h"
#include "SSConatantBufferData.h"

using namespace DirectX;

enum class EInstanceDataType : unsigned char
{
    IDT_FLOAT2,
    IDT_FLOAT3,
    IDT_FLOAT4,
};

struct SSInstanceData
{
public:
    std::vector<XMFLOAT2> Float2InstancedData;
    std::vector<XMFLOAT3> Float3InstancedData;
    std::vector<XMFLOAT4> Float4InstancedData;

    EInstanceDataType InstanceDataType = EInstanceDataType::IDT_FLOAT3;
};

enum class EVertexType : unsigned char
{
	VT_PNT, // Position, Normal, Texture
	VT_PNTT, // Position, Normal, Texture, Tangent
	VT_PT, //
};


struct SSMeshVertexIndexData
{
public:
	// vertex datas
	unsigned int Stride = 0;
	unsigned int Count	= 0;
	void* DataPtr = nullptr;
	
	// index datas 
    std::vector<unsigned int> IndexData;	
	D3D_PRIMITIVE_TOPOLOGY PrimitiveType = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	bool bHasIndexData = false;
};

// graphics api independent render data (whether it's opengl or directx...)
struct SSMeshRenderData
{
public:
	std::string VertexShaderName;
	std::string PixelShaderName;
	//
	std::map<std::string, SSConatantBufferData> VSConstantBufferMap;
	std::map<std::string, SSConatantBufferData> PSConstantBufferMap;

	std::map<std::string, std::string> VSTextureMap;
	std::map<std::string, std::string> PSTextureMap;
};

struct SSMeshElementRenderData
{
	SSName VertexShaderName;
	SSName PixelShaderName;
	//
	std::map<SSName, SSConatantBufferData> VSConstantBufferMap;
	std::map<SSName, SSConatantBufferData> PSConstantBufferMap;

	std::map<SSName, SSName> VSTextureMap;
	std::map<SSName, SSName> PSTextureMap;

	unsigned int StartIndex = 0;
	unsigned int EndIndex = 0;
};
