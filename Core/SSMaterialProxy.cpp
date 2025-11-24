
#include "SSMaterialProxy.h"

SSMaterialProxy::SSMaterialProxy(std::string InVSName, std::string InPSName)
	: VertexShaderName(InVSName), PixelShaderName(InPSName)
{
}

void SSMaterialProxy::SetVSConstantParam(const std::string& InName, const SSConstantBufferData& InData)
{
	VSConstantBufferMap[InName] = InData;
}

void SSMaterialProxy::SetVSTextureParam(const std::string& InName, const std::string& InTextureName)
{
	VSTextureMap[InName] = InTextureName;
}

void SSMaterialProxy::SetPSConstantParam(const std::string& InName, const SSConstantBufferData& InData)
{
	PSConstantBufferMap[InName] = InData;
}

void SSMaterialProxy::SetPSTextureParam(const std::string& InName, const std::string& InTextureName)
{
	PSTextureMap[InName] = InTextureName;
}

SSMaterialProxy::~SSMaterialProxy()
{
}
