#include "SSSkybox.h"
#include "SSSharedRenderData.h"

SSSkybox::SSSkybox()
{
	CreateRenderData();
	CreateVertexData();
}

SSSkybox::~SSSkybox()
{
}

void SSSkybox::CreateVertexData()
{
	mVertexData.bHasIndexData = false;
	SSSimpleVertexData VertexData = SSSharedRenderData::Get().GetSphereVertexData();
	mVertexData.Count = VertexData.Count;
	mVertexData.Stride = VertexData.Stride;
	mVertexData.DataPtr = VertexData.DataPtr;
}

void SSSkybox::CreateRenderData()
{
	mMaterialProxy->SetPixelShaderName("CubemapShader.ps");
	mMaterialProxy->SetVertexShaderName("CubemapShader.vs");
	mMaterialProxy->SetPSTextureParam("gCubeMap", "./Prebaked/EnvCubemap.dds");
}