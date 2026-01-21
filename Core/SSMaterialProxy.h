#pragma once

#include "SSConstantBufferData.h"
#include <memory>

class CORE_API SSMaterialProxy : public std::enable_shared_from_this<SSMaterialProxy>
{
public:

	SSMaterialProxy(std::string InVSName, std::string InPSName);
	SSMaterialProxy() = default;
	SSMaterialProxy(const SSMaterialProxy& rhs) = default;
	~SSMaterialProxy();

	void SetVSConstantParam(const std::string& InName, const SSConstantBufferData& InData);
	const SSConstantBufferData* GetVSConstantParam(const std::string& InName) const;
	
	void SetPSConstantParam(const std::string& InName, const SSConstantBufferData& InData);
	const SSConstantBufferData* GetPSConstantParam(const std::string& InName) const;

	void SetVSTextureParam(const std::string& InName, const std::string& InTextureName);
	void SetPSTextureParam(const std::string& InName, const std::string& InTextureName);

	void SetVertexShaderName(const std::string& InVSName) { VertexShaderName = InVSName; }
	void SetPixelShaderName(const std::string& InPSName) { PixelShaderName = InPSName; }

	inline std::string GetVertexShaderName() const { return VertexShaderName; }
	inline std::string GetPixelShaderName() const { return PixelShaderName; }

	const std::map<std::string, SSConstantBufferData>& GetVSConstantBufferMap() const { return VSConstantBufferMap; }
	const std::map<std::string, SSConstantBufferData>& GetPSConstantBufferMap() const { return PSConstantBufferMap; }

	const std::map<std::string, unsigned int>& GetVSStructuredBufferSlotMap() const { return VSStructuredBufferSlotMap; }
	const std::map<std::string, unsigned int>& GetPSStructuredBufferSlotMap() const { return PSStructuredBufferSlotMap; }

	const std::map<std::string, std::string>& GetVSTextureMap() const { return VSTextureMap; }
	const std::map<std::string, std::string>& GetPSTextureMap() const { return PSTextureMap; }

private:
	std::string VertexShaderName;
	std::string PixelShaderName;

	std::map<std::string, SSConstantBufferData> VSConstantBufferMap;
	std::map<std::string, SSConstantBufferData> PSConstantBufferMap;

	std::map < std::string, unsigned int> VSStructuredBufferSlotMap;
	std::map < std::string, unsigned int> PSStructuredBufferSlotMap;

	std::map<std::string, std::string> VSTextureMap;
	std::map<std::string, std::string> PSTextureMap;

};

