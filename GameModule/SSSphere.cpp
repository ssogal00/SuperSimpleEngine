
#include "SSSphere.h"
#include <cmath>
#include "SSVertexTypes.h"
#include "SSFreqUsedNames.h"
#include "SSName.h"
#include "SSCameraManager.h"
#include "SSMathHelper.h"
#include "SSSharedRenderData.h"
#include "FreqUsedConstantBufferTypes.h"

SSSphere::SSSphere(UINT sector, UINT stack, float radius)
	: mSectorCount(sector), mStackCount(stack), mRadius(radius)
{	
	CreateVertexData();
	CreateRenderData();	
}

void SSSphere::CreateVertexData()
{
	mVertexData.bHasIndexData = false;
    SSSimpleVertexData VertexData  = SSSharedRenderData::Get().GetSphereVertexData();
	mVertexData.Count = VertexData.Count;
	mVertexData.Stride = VertexData.Stride;
	mVertexData.DataPtr = VertexData.DataPtr;

}

void SSSphere::CreateRenderData()
{
	SSAlignedCBuffer<int, int, int, int, int> settings;	
	
	mMaterialProxy->SetPixelShaderName("GBuffer.ps");
	mMaterialProxy->SetVertexShaderName("GBuffer.vs");
	mMaterialProxy->SetPSTextureParam("DiffuseTex", "./Resource/Tex/rustediron/rustediron2_basecolor.dds");
	mMaterialProxy->SetPSTextureParam("NormalTex", "./Resource/Tex/rustediron/rustediron2_normal.dds");
	mMaterialProxy->SetPSTextureParam("MetalicTex", "./Resource/Tex/rustediron/rustediron2_metallic.dds");
	mMaterialProxy->SetPSTextureParam("RoughnessTex", "./Resource/Tex/rustediron/rustediron2_roughness.dds");
		
	settings.value1 = 1; //metalic
	settings.value2 = 0; //mask
	settings.value3 = 1; //normal
	settings.value4 = 1; // roghness
	settings.value5 = 1; // diffuse

	SSConstantBufferData Data{ settings };
	mMaterialProxy->SetPSConstantParam("TextureExist", Data);
}

void SSSphere::Tick(float delta)
{
	SSGameObject::Tick(delta);

    mYaw += 1.f * delta;
}

std::vector< VT_PositionNormalTexcoordTangent > SSSphere::mVertexArray;

SSPBRSphere::SSPBRSphere()
: SSSphere(25,25,10)
{
    mNormalTexName = "./Resource/Tex/metalgrid/metalgrid4_normal-dx.dds";
    mDiffuseTexName = "./Resource/Tex/metalgrid/metalgrid4_basecolor.dds";
    mMetalTexName = "./Resource/Tex/metalgrid/metalgrid4_metallic.dds";
    mRoughTexName = "./Resource/Tex/metalgrid/metalgrid4_roughness.dds";

    CreateRenderData();
}

SSPBRSphere::SSPBRSphere(SSName diffuseTexName, SSName normalTexName, SSName metalTexName, SSName roughTexName)
	: SSSphere(25, 25, 10.f)
{
	mDiffuseTexName = diffuseTexName;
	mNormalTexName = normalTexName;
	mMetalTexName = metalTexName;
	mRoughTexName = roughTexName;

	CreateRenderData();
}

void SSPBRSphere::Tick(float delta)
{
	SSGameObject::Tick(delta);

	mYaw -= 1.f * delta;
}

void SSPBRSphere::CreateRenderData()
{
	mMaterialProxy->SetVertexShaderName("GBuffer.vs");
	mMaterialProxy->SetPixelShaderName("GBuffer.ps");   

	mMaterialProxy->SetPSTextureParam("DiffuseTex", mDiffuseTexName);
	mMaterialProxy->SetPSTextureParam("NormalTex", mNormalTexName);
	mMaterialProxy->SetPSTextureParam("MetalicTex", mMetalTexName);
	mMaterialProxy->SetPSTextureParam("RoughnessTex", mRoughTexName);
}


SSInstancedSphere::SSInstancedSphere( UINT sector, UINT stack, float radius, const SSInstancedVertexData& instancedData)
: SSSphere(sector,stack, radius)
, mInstanceData(instancedData)
{
	CreateVertexData();
	CreateRenderData();
}

void SSInstancedSphere::CreateRenderData()
{
	mMaterialProxy->SetVertexShaderName("DeferredInstanced.vs");
	mMaterialProxy->SetPixelShaderName("GBuffer.ps");
	
	SSAlignedCBuffer<int, int, int, int, int> settings;

	settings.value1 = 1; //metalic
	settings.value2 = 0; //mask
	settings.value3 = 1; //normal
	settings.value4 = 1; // roghness
	settings.value5 = 1; // diffuse
	SSConstantBufferData Data{ settings };

	mMaterialProxy->SetPSTextureParam("DiffuseTex", "./Resource/Tex/rustediron/rustediron2_basecolor.dds");
	mMaterialProxy->SetPSTextureParam("NormalTex", "./Resource/Tex/rustediron/rustediron2_normal.dds");
	mMaterialProxy->SetPSTextureParam("MetalicTex", "./Resource/Tex/rustediron/rustediron2_metallic.dds");
	mMaterialProxy->SetPSTextureParam("RoughnessTex", "./Resource/Tex/rustediron/rustediron2_roughness.dds");

	mMaterialProxy->SetPSConstantParam("TextureExist", Data);
}

void SSInstancedSphere::CreateVertexData()
{
	SSSimpleVertexData VertexData = SSSharedRenderData::Get().GetSphereVertexData();

	mVertexData.Count = VertexData.Count;
	mVertexData.Stride = VertexData.Stride;
	mVertexData.DataPtr = VertexData.DataPtr;
	mVertexData.bHasIndexData = false;

	mVertexData.bHasInstanceData = true;
	mVertexData.InstanceCount = mInstanceData.InstanceCount;
	mVertexData.InstanceStride = mInstanceData.InstanceStride;
	mVertexData.InstanceDataPtr = mInstanceData.InstanceDataPtr;
}