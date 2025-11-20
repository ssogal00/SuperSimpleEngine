#pragma once

#include "SSRendererModulePCH.h"
#include "SSDX11ConstantBuffer.h"
#include "SSName.h"
#include "wrl/client.h"
#include <map>
#include <string_view>

using namespace std;
using Microsoft::WRL::ComPtr;

class SSCompileContext
{
public:
    vector<pair<string_view, string_view>> MacroDefines;
    vector<string_view> InstancedAttributes;
};

//class ENGINE_API SSShader : public SSRenderThreadObject
class SSShader
{
public:
	virtual ~SSShader(){}

    template<class T>
    void SetConstantBufferData(std::string bufferName, const T& data);	

    virtual SSDX11ConstantBuffer* GetConstantBuffer(std::string bufferName) ;

	virtual void SetTexture(ID3D11DeviceContext* deviceContext, std::string name, class SSDX11Texture2D* textrue ){}	
	virtual void SetTextureAsNull(ID3D11DeviceContext* deviceContext, std::string name){}
	virtual void SetTextureAsNull(std::string name){}
	
	virtual void SetSampler(ID3D11DeviceContext* device, std::string name, ID3D11SamplerState* sampler) {}
	virtual void SetSampler(std::string name, ID3D11SamplerState* sampler) {}

	std::vector<std::string> GetSamplerNames();

	inline std::map<std::string, SSDX11ConstantBuffer*> GetConstantBufferMap() { return mConstantBufferMap; }

	ID3DBlob* GetCompiledShader() { return mShaderBuffer.Get(); }

    virtual bool CompileFromFile(std::wstring filepath) { return true; }

    virtual bool CompileFromFile(std::wstring filepath, const SSCompileContext& context) {return true;}

	int GetConstantBufferSlotIndex(const std::string& InName);

	virtual int GetTextureSlotIndex(const std::string& InName);

	virtual int GetSamplerSlotIndex(const std::string& InName);
protected:
	
	virtual void ReflectCompiledShader(ID3D11ShaderReflection* reflection);

    void PrintCompileError(ID3DBlob* errorMsg);
    
	ComPtr<ID3DBlob> mShaderBuffer = nullptr;

	std::map<std::string, SSDX11ConstantBuffer*> mConstantBufferMap;
	std::map<std::string, UINT> mTextureMap;
	std::map<std::string, UINT> mSamplerMap;

	std::map<std::string, ID3D11ShaderResourceView*> mBoundTextureMap;

	UINT mConstantBufferCount = 0;
	UINT mBoundResourceCount = 0;
	D3D11_SHADER_VERSION_TYPE mShaderType;
};

template<class T>
void SSShader::SetConstantBufferData(std::string bufferName, const T& data)
{
    if(mConstantBufferMap.count(bufferName) > 0)
    {
		auto* buffer = mConstantBufferMap[bufferName];
		buffer->SetBufferData<T>(data);
    }
}


// vertex shader
class SSDX11VertexShader : public SSShader
{
public:
    SSDX11VertexShader() = default;
	virtual ~SSDX11VertexShader();	
    virtual bool CompileFromFile(std::wstring filepath) override;
    virtual bool CompileFromFile(std::wstring filepath, const SSCompileContext& context) override;
    ID3D11VertexShader* GetShader() { return mVertexShader; } 
    ID3D11InputLayout* GetInputLayout() { return mInputLayout; }

	template<class T>
	void SetConstantBufferData(ID3D11DeviceContext* deviceContext, std::string bufferName, const T& data);

	virtual void SetTexture(ID3D11DeviceContext* deviceContext, std::string name, class SSDX11Texture2D* texture) override;

	virtual void SetSampler(ID3D11DeviceContext* device, std::string name, ID3D11SamplerState* sampler) override;	
	
	virtual void SetSampler(std::string name, ID3D11SamplerState* sampler) override;

	D3D_PRIMITIVE_TOPOLOGY GetPrimitiveType() const { return mPrimitiveType; }
protected:	
    void CreateInputLayout(ID3D11ShaderReflection* shaderReflection, const SSCompileContext& context);
    ID3D11VertexShader* mVertexShader = nullptr;
    ID3D11InputLayout* mInputLayout = nullptr;
	D3D_PRIMITIVE_TOPOLOGY mPrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

template<class T>
void SSDX11VertexShader::SetConstantBufferData(ID3D11DeviceContext* deviceContext, std::string bufferName, const T& data)
{	
	if (mConstantBufferMap.count(bufferName) > 0)
	{
		mConstantBufferMap[bufferName]->SetBufferData<T>(deviceContext, data);
		UINT bufferIndex = mConstantBufferMap[bufferName]->GetBufferIndex();
		deviceContext->VSSetConstantBuffers(bufferIndex, 1, &mConstantBufferMap[bufferName]->GetBufferPointerRef());
	}
}

// pixel shader
class SSDX11PixelShader : public SSShader
{
public:
    SSDX11PixelShader() = default;
	virtual ~SSDX11PixelShader();	
    virtual bool CompileFromFile(std::wstring filepath) override;
    virtual bool CompileFromFile(std::wstring filepath, const SSCompileContext& context) override;

	ID3D11PixelShader* GetShader()	{return mPixelShader;}

	template<class T>
	void SetConstantBufferData(ID3D11DeviceContext* deviceContext, std::string bufferName, const T& data);

	virtual void SetTextureAsNull(ID3D11DeviceContext* deviceContext, std::string name)override;
	virtual void SetTexture(ID3D11DeviceContext* deviceContext, std::string name, class SSDX11Texture2D* texture) override;
	virtual void SetTextureAsNull(std::string name) override;
	virtual void SetSampler(std::string name, ID3D11SamplerState* sampler) override;
	virtual void SetSampler(ID3D11DeviceContext* device, std::string name, ID3D11SamplerState* sampler) override;
protected:

	ID3D11PixelShader* mPixelShader;
};

template<class T>
void SSDX11PixelShader::SetConstantBufferData(ID3D11DeviceContext* deviceContext, std::string bufferName, const T& data)
{
	if (mConstantBufferMap.count(bufferName) > 0)
	{
		mConstantBufferMap[bufferName]->SetBufferData<T>(deviceContext, data);
		UINT bufferIndex = mConstantBufferMap[bufferName]->GetBufferIndex();
		deviceContext->PSSetConstantBuffers(
			bufferIndex,
			1,
			&mConstantBufferMap[bufferName]->GetBufferPointerRef());
	}
}
