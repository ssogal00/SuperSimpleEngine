
#include "SSGameModule.h"
#include "SSMaterialProxy.h"

SSMaterialProxy::SSMaterialProxy(std::string InVSName, std::string InPSName)
	: VertexShaderName(InVSName), PixelShaderName(InPSName)
{
}

void SSMaterialProxy::SetConstantParam(const std::string& InName, const SSConatantBufferData& InData)
{
	if(VSConstantBufferMap.count(InName) > 0)
	{
		VSConstantBufferMap[InName] = InData;
	}
	else if(PSConstantBufferMap.count(InName) > 0)
	{
		PSConstantBufferMap[InName] = InData;
	}
}

void SSMaterialProxy::SetTextureParam(const std::string& InName, const std::string& InTextureName)
{
	if (VSTextureMap.count(InName) > 0)
	{
		VSTextureMap[InName] = InTextureName;
	}
	else if (PSTextureMap.count(InName) > 0)
	{
		PSTextureMap[InName] = InTextureName;
	}
}

SSMaterialProxy::~SSMaterialProxy()
{
}
