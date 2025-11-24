
#include "SSCube.h"
#include "SSVertexTypes.h"
#include "SSSharedRenderData.h"
#include "FreqUsedConstantBufferTypes.h"
#include "SSName.h"

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
	mRenderData.VertexShaderName = "GBuffer.vs";
	mRenderData.PixelShaderName = "GBuffer.ps";
	mRenderData.PSTextureMap["DiffuseTex"] = "./Resource/Tex/rustediron/rustediron2_basecolor.dds";
	mRenderData.PSTextureMap["NormalTex"] = "./Resource/Tex/rustediron/rustediron2_normal.dds";
	mRenderData.PSTextureMap["MetalicTex"] = "./Resource/Tex/rustediron/rustediron2_metallic.dds";
	mRenderData.PSTextureMap["RoughnessTex"] = "./Resource/Tex/rustediron/rustediron2_roughness.dds";		

	SSAlignedCBuffer<int, int, int, int, int> settings;
	settings.value1 = 1; //metalic
	settings.value2 = 0; //mask
	settings.value3 = 1; //normal
	settings.value4 = 1; // roghness
	settings.value5 = 1; // diffuse

	SSConstantBufferData proxy{ settings };	
	
	mRenderData.PSConstantBufferMap["TextureExist"] = std::move(proxy);
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

