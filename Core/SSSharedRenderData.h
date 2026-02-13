

#pragma once
#include "Singleton.h"
#include "SSVertexTypes.h"
#include <vector>

struct CORE_API SSSimpleVertexData
{
	unsigned int Count = 0;
	unsigned int Stride = 0;
	void* DataPtr = nullptr;
};

class CORE_API SSSharedRenderData : public Singleton<SSSharedRenderData>
{
public:
	void Initialize();

	SSSimpleVertexData GetCubeVertexData();	
	SSSimpleVertexData GetSphereVertexData();
	SSSimpleVertexData GetScreenBlitVertexData();
	
	std::vector<UINT>& GetCubeIndexData() { return mCubeIndexData; }	
	std::vector<UINT>& GetSphereIndexData() { return mSphereIndexData; }
	std::vector<UINT>& GetScreenBlitIndexData() {return mScreenBlitIndexData;}

	// 
	std::vector<VT_PositionNormalTexcoordTangent> GetRawCubeVertexData() { return mCubeVertexData; }
	std::vector<VT_PositionNormalTexcoordTangent> GetRawSphereVertexData() { return mSphereVertexData; }
	
protected:
	void CreateCubeVertexData();
	void CreateSphereVertexData();
	void CreateScreenBlitVertexData();

	std::vector<DirectX::XMFLOAT4> GenerateTangents(const std::vector<DirectX::XMFLOAT4>& tempVertexList,
		const std::vector<DirectX::XMFLOAT3>& tempNormalList,
		const std::vector<DirectX::XMFLOAT2>& tempTexCoordList);
	
	std::vector<VT_PositionNormalTexcoordTangent> mCubeVertexData;
	std::vector<VT_PositionNormalTexcoordTangent> mSphereVertexData;
	std::vector<VT_PositionTexcoord> mScreenBlitVertexData;
	std::vector<UINT> mScreenBlitIndexData;

	std::vector<UINT> mCubeIndexData;	
	std::vector<UINT> mSphereIndexData;
};
