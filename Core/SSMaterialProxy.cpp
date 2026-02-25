
#include "SSMaterialProxy.h"

SSMaterialProxy::SSMaterialProxy(std::string InVSName, std::string InPSName)
	: VertexShaderName(InVSName), PixelShaderName(InPSName)
{
}
const SSConstantBufferData* SSMaterialProxy::GetVSConstantParam(const std::string& InName) const
{
	auto It = VSConstantBufferMap.find(InName);
	if (It != VSConstantBufferMap.end())
	{
		return &It->second;
	}
	return nullptr;
}


const SSConstantBufferData* SSMaterialProxy::GetPSConstantParam(const std::string& InName) const
{
	auto It = PSConstantBufferMap.find(InName);
	if (It != PSConstantBufferMap.end())
	{
		return &It->second;
	}
	return nullptr;
}

void SSMaterialProxy::SetVSConstantParam(const std::string& InName, const SSConstantBufferData& InData)
{
	VSConstantBufferMap[InName] = InData;
}

void SSMaterialProxy::SetVSTextureParam(const std::string& InName, const std::string& InTextureName, bool bSRGB)
{
	VSTextureMap[InName] = InTextureName;
	if(bSRGB)
	{
		SRGBTextureSet.insert(InTextureName);
	}
	else
	{
		SRGBTextureSet.erase(InTextureName);
	}
}

void SSMaterialProxy::SetPSConstantParam(const std::string& InName, const SSConstantBufferData& InData)
{
	PSConstantBufferMap[InName] = InData;
}

void SSMaterialProxy::SetPSTextureParam(const std::string& InName, const std::string& InTextureName, bool bSRGB)
{
	PSTextureMap[InName] = InTextureName;
	if (bSRGB)
	{
		SRGBTextureSet.insert(InTextureName);
	}
	else
	{
		SRGBTextureSet.erase(InTextureName);
	}
}

SSMaterialProxy::~SSMaterialProxy()
{
}


bool SSMaterialProxy::IsSRGBTexture(const std::string& InName) const
{
	if(SRGBTextureSet.count(InName) > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
