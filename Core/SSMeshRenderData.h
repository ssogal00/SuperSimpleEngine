#pragma once

#include "SSVertexTypes.h"
#include <vector>
#include "SSConstantBufferData.h"

using namespace DirectX;


enum class EVertexType : unsigned char
{
	VT_PNT, // Position, Normal, Texture
	VT_PNTT, // Position, Normal, Texture, Tangent
	VT_PT, //
};

struct SSInstancedVertexData
{
public:	
	SSInstancedVertexData(void* InInstanceDataPtr, unsigned int InInstanceStride, unsigned int InInstanceCount)
		:InstanceDataPtr(InInstanceDataPtr)
		,InstanceStride(InInstanceStride)
		,InstanceCount(InInstanceCount)
	{
	}

	SSInstancedVertexData() = default;
	SSInstancedVertexData(const SSInstancedVertexData& rhs) = default;


	void* InstanceDataPtr = nullptr;
	unsigned int InstanceStride = 0;
	unsigned int InstanceCount = 0;
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

	// instanced data
	unsigned int InstanceStride = 0;
	unsigned int InstanceCount = 0;
	void* InstanceDataPtr = nullptr;
	bool bHasInstanceData = false;
};

// graphics api independent render data (whether it's opengl or directx...)
struct SSMeshRenderData
{
public:
	std::string VertexShaderName;
	std::string PixelShaderName;
	//
	std::map<std::string, SSConstantBufferData> VSConstantBufferMap;
	std::map<std::string, SSConstantBufferData> PSConstantBufferMap;

	std::map<std::string, std::string> VSTextureMap;
	std::map<std::string, std::string> PSTextureMap;
};
