#pragma once

#include "SSConstantBufferData.h"

class GAMEMODULE_API SSMaterialProxy
{
public:

	SSMaterialProxy(std::string InVSName, std::string InPSName);
	~SSMaterialProxy();

	void SetConstantParam(const std::string& InName, const SSConstantBufferData& InData);
	void SetTextureParam(const std::string& InName, const std::string& InTextureName);

private:
	std::string VertexShaderName;
	std::string PixelShaderName;

	std::map<std::string, SSConstantBufferData> VSConstantBufferMap;
	std::map<std::string, SSConstantBufferData> PSConstantBufferMap;

	std::map<std::string, std::string> VSTextureMap;
	std::map<std::string, std::string> PSTextureMap;

};

