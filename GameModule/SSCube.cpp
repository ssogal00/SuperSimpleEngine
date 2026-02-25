
#include "SSCube.h"
#include "SSVertexTypes.h"
#include "SSSharedRenderData.h"
#include "FreqUsedConstantBufferTypes.h"


SSCube::SSCube()
{
	CreateVertexData();
	CreateRenderData();
	SetScale(5.f,5.f,5.f);
	SetPositionZ(1);	
}

void SSCube::Tick(float delta)
{
	SSGameObject::Tick(delta);

	mYaw += 3.f * delta;
}

void SSCube::CreateRenderData()
{		
	mMaterialProxy->SetVertexShaderName("GBuffer.vs");
	mMaterialProxy->SetPixelShaderName("GBuffer.ps");
	mMaterialProxy->SetPSTextureParam("DiffuseTex", "./Resource/Tex/rustediron/rustediron2_basecolor.dds", true);
	mMaterialProxy->SetPSTextureParam("NormalTex", "./Resource/Tex/rustediron/rustediron2_normal.dds", false);
	mMaterialProxy->SetPSTextureParam("MetalicTex", "./Resource/Tex/rustediron/rustediron2_metallic.dds", false);
	mMaterialProxy->SetPSTextureParam("RoughnessTex", "./Resource/Tex/rustediron/rustediron2_roughness.dds", false);

	SSAlignedCBuffer<int, int, int, int, int> settings;
	settings.value1 = 1; //metalic
	settings.value2 = 0; //mask
	settings.value3 = 1; //normal
	settings.value4 = 1; // roghness
	settings.value5 = 1; // diffuse

	SSConstantBufferData proxy{ settings };	

	mMaterialProxy->SetPSConstantParam("TextureExist", proxy);
}

void SSCube::CreateVertexData()
{		
	mVertexData.IndexData = SSSharedRenderData::Get().GetCubeIndexData();
	mVertexData.bHasIndexData = true;
	mVertexData.PrimitiveType = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;	
	SSSimpleVertexData Data = SSSharedRenderData::Get().GetCubeVertexData();
	mVertexData.Stride = Data.Stride;
	mVertexData.Count = Data.Count;
	mVertexData.DataPtr = Data.DataPtr;
}

