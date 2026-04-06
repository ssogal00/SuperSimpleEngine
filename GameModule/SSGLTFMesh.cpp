

#include "SSGLTFMesh.h"
#include "SSVertexTypes.h"
#include "FreqUsedConstantBufferTypes.h"
#include "SSSharedRenderData.h"


SSGLTFMeshObject::SSGLTFMeshObject(std::string InGLTFFilePath)
	: mGLTFFilePath(InGLTFFilePath)
{
	 mGLTFData = GLTF::SSGLTF_V2::LoadGLTFFile(InGLTFFilePath);

	 mMaterialProxy->SetPixelShaderName("GBufferGLTF.ps.default");
	 mMaterialProxy->SetVertexShaderName("DeferredInstancedManualFetch.vs");
}

SSGLTFMeshObject::~SSGLTFMeshObject()
{

}


////
SSGLTFTestObject::SSGLTFTestObject()
{
	std::vector< VT_PositionNormalTexcoordTangent> CubeDataList = SSSharedRenderData::Get().GetRawCubeVertexData();

	for(VT_PositionNormalTexcoordTangent& vertex : CubeDataList)
	{
		mGLTFData.MergedPositions.push_back(XMFLOAT3(
			vertex.VertexAttribute1.x,
			vertex.VertexAttribute1.y-1,
			vertex.VertexAttribute1.z
		));

		mGLTFData.MergedNormals.push_back(vertex.VertexAttribute2);
		mGLTFData.MergedTexcoords.push_back(vertex.VertexAttribute3);
		mGLTFData.MergedTangents.push_back(vertex.VertexAttribute4);
	}

	

	mGLTFData.PositionCountList.push_back(static_cast<unsigned int>(CubeDataList.size()));

	std::vector<VT_PositionNormalTexcoordTangent> SphereDataList = SSSharedRenderData::Get().GetRawCubeVertexData();

	for (VT_PositionNormalTexcoordTangent& vertex : SphereDataList)
	{
		XMFLOAT3 OffsetPos = XMFLOAT3(
			vertex.VertexAttribute1.x ,
			vertex.VertexAttribute1.y + 1.2,
			vertex.VertexAttribute1.z
		);
		mGLTFData.MergedPositions.push_back(OffsetPos);
		mGLTFData.MergedNormals.push_back(vertex.VertexAttribute2);
		mGLTFData.MergedTexcoords.push_back(vertex.VertexAttribute3);
		mGLTFData.MergedTangents.push_back(vertex.VertexAttribute4);
	}

	mGLTFData.PositionCountList.push_back(static_cast<unsigned int>(SphereDataList.size()));

	auto CubeIndexData = SSSharedRenderData::Get().GetCubeIndexData();

	uint16_t PrevIndexSize = static_cast<uint16_t>(SphereDataList.size());
	
	for (UINT index : CubeIndexData)
	{
		mGLTFData.MergedIndices.push_back(static_cast<uint16_t>(index));
	}
	mGLTFData.IndexCountList.push_back(static_cast<unsigned int>(CubeIndexData.size()));

	auto SphereIndexData = SSSharedRenderData::Get().GetCubeIndexData();

	for (UINT index : SphereIndexData)
	{
		mGLTFData.MergedIndices.push_back(static_cast<uint16_t>(index) + PrevIndexSize);
		//mGLTFData.MergedIndices.push_back(static_cast<uint16_t>(index));
	}
	
	mGLTFData.IndexCountList.push_back(static_cast<unsigned int>(SphereIndexData.size()));

	mMaterialProxy->SetPixelShaderName("GBufferGLTF.ps.default");
	mMaterialProxy->SetVertexShaderName("DeferredInstancedManualFetch.vs");
	mMaterialProxy->SetPSTextureParam("DiffuseTex", "./Resource/Tex/rustediron/rustediron2_basecolor.dds", true);
	mMaterialProxy->SetPSTextureParam("NormalTex", "./Resource/Tex/rustediron/rustediron2_normal.dds",false);
	mMaterialProxy->SetPSTextureParam("MetalicTex", "./Resource/Tex/rustediron/rustediron2_metallic.dds",false);
	mMaterialProxy->SetPSTextureParam("RoughnessTex", "./Resource/Tex/rustediron/rustediron2_roughness.dds",false);

	SSAlignedCBuffer<int, int, int, int, int> settings;

	settings.value1 = 1; //metalic
	settings.value2 = 0; //mask
	settings.value3 = 1; //normal
	settings.value4 = 1; // roghness
	settings.value5 = 1; // diffuse

	SSConstantBufferData Data{ settings };
	mMaterialProxy->SetPSConstantParam("TextureExist", Data);
}