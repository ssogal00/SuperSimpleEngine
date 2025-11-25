#pragma once

#include "SSConstantBufferData.h"

class CORE_API SSMaterialProxy
{
public:

	SSMaterialProxy(std::string InVSName, std::string InPSName);
	SSMaterialProxy() = default;
	SSMaterialProxy(const SSMaterialProxy& rhs) = default;
	~SSMaterialProxy();

	void SetVSConstantParam(const std::string& InName, const SSConstantBufferData& InData);
	const SSConstantBufferData* GetVSConstantParam(const std::string& InName) const;

	void SetVSTextureParam(const std::string& InName, const std::string& InTextureName);
	void SetPSConstantParam(const std::string& InName, const SSConstantBufferData& InData);
	void SetPSTextureParam(const std::string& InName, const std::string& InTextureName);
	void SetVertexShaderName(const std::string& InVSName) { VertexShaderName = InVSName; }
	void SetPixelShaderName(const std::string& InPSName) { PixelShaderName = InPSName; }

	const std::map<std::string, SSConstantBufferData>& GetVSConstantBufferMap() const { return VSConstantBufferMap; }

private:
	std::string VertexShaderName;
	std::string PixelShaderName;

	std::map<std::string, SSConstantBufferData> VSConstantBufferMap;
	std::map<std::string, SSConstantBufferData> PSConstantBufferMap;

	std::map<std::string, std::string> VSTextureMap;
	std::map<std::string, std::string> PSTextureMap;

};

