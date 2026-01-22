

#include "SSGLTFMesh.h"
#include "SSVertexTypes.h"
#include "FreqUsedConstantBufferTypes.h"


SSGLTFMeshObject::SSGLTFMeshObject(std::string InGLTFFilePath)
	: mGLTFFilePath(InGLTFFilePath)
{
	 mGLTFData = GLTF::SSGLTF_V2::LoadGLTFFile(InGLTFFilePath);

	 mMaterialProxy->SetPixelShaderName("GBuffer.ps");
	 mMaterialProxy->SetVertexShaderName("DeferredInstancedManualFetch.vs");
	 mMaterialProxy->SetPSTextureParam("DiffuseTex", "./Resource/Tex/rustediron/rustediron2_basecolor.dds");
	 mMaterialProxy->SetPSTextureParam("NormalTex", "./Resource/Tex/rustediron/rustediron2_normal.dds");
	 mMaterialProxy->SetPSTextureParam("MetalicTex", "./Resource/Tex/rustediron/rustediron2_metallic.dds");
	 mMaterialProxy->SetPSTextureParam("RoughnessTex", "./Resource/Tex/rustediron/rustediron2_roughness.dds");
	 
	 SSAlignedCBuffer<int, int, int, int, int> settings;

	 settings.value1 = 1; //metalic
	 settings.value2 = 0; //mask
	 settings.value3 = 1; //normal
	 settings.value4 = 1; // roghness
	 settings.value5 = 1; // diffuse

	 SSConstantBufferData Data{ settings };
	 mMaterialProxy->SetPSConstantParam("TextureExist", Data);
}

SSGLTFMeshObject::~SSGLTFMeshObject()
{

}
