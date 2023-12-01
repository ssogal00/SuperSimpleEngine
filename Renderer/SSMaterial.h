#pragma once

#include "SSRenderThreadObject.h"
#include "SSName.h"
#include "SSVertexTypes.h"
#include "SSConatantBufferData.h"
#include "SSDX11ConstantBuffer.h"

using namespace std;

class SSDX11VertexShader;
class SSDX11PixelShader;
class SSDX11Texture2D;


class DX11RENDERER_API SSMaterial : public SSRenderThreadResidentObject
{
public:	
	SSMaterial(std::shared_ptr<SSDX11VertexShader> vs, std::shared_ptr<SSDX11PixelShader> ps);

	virtual void SetCurrent();
	virtual void ReleaseCurrent();

	template<class T>
	void SetVSConstantBufferData(ID3D11DeviceContext* deviceContext, SSName name, const T& value);
	template<class T>
	void SetPSConstantBufferData(ID3D11DeviceContext* deviceContext, SSName name, const T& value);

	void SetVSConstantBufferProxyData(ID3D11DeviceContext* deviceContext, SSName name, const SSConatantBufferData& buffer);
	void SetPSConstantBufferProxyData(ID3D11DeviceContext* deviceContext, SSName name, const SSConatantBufferData& buffer);
	

	template<class T>
	void SetVSConstantBufferDataChecked(ID3D11DeviceContext* deviceContext, SSName name, const T& value);

	void SetPSTexture(ID3D11DeviceContext* deviceContext, std::string name, SSDX11Texture2D* texture);
	void SetVSTexture(ID3D11DeviceContext* deviceContext, std::string name, SSDX11Texture2D* texture);

	virtual void SetPSSampler(std::string name, ID3D11SamplerState* sampler);

	void SetPrimitiveType(D3D_PRIMITIVE_TOPOLOGY ePrimitiveType) { mPrimitiveType = ePrimitiveType; }
	D3D_PRIMITIVE_TOPOLOGY GetPrimitiveType() const { return mPrimitiveType; }	

protected:	

	std::shared_ptr<class SSDX11VertexShader> mVS;
	std::shared_ptr<class SSDX11PixelShader> mPS;

	//
	std::map<SSName, class SSGenericConstantBuffer*> mVertexShaderConstantBufferMap;
	std::map<SSName, class SSGenericConstantBuffer*> mPixelShaderConstantBufferMap;

	//
	std::map<std::string, class SSDX11Texture2D*> mPixelShaderTextureMap;
	std::map<std::string, class SSDX11Texture2D*> mVertexShaderTextureMap;

	//
	D3D_PRIMITIVE_TOPOLOGY mPrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};


template<class T>
void SSMaterial::SetVSConstantBufferData(ID3D11DeviceContext* deviceContext, SSName name, const T& value)
{
	if (mVertexShaderConstantBufferMap.count(name) > 0)
	{
		mVertexShaderConstantBufferMap[name]->StoreBufferData(value);
		mVertexShaderConstantBufferMap[name]->SubmitDataToDevice(deviceContext);
		
		UINT bufferIndex = mVertexShaderConstantBufferMap[name]->GetBufferIndex();

		check(deviceContext != nullptr);

		deviceContext->VSSetConstantBuffers(bufferIndex, 1, (ID3D11Buffer* const*)mVertexShaderConstantBufferMap[name]->GetBufferPointerRef());
	}
}

template<class T>
void SSMaterial::SetVSConstantBufferDataChecked(ID3D11DeviceContext* deviceContext, SSName name, const T& value)
{
	if (mVertexShaderConstantBufferMap.count(name) > 0)
	{
		mVertexShaderConstantBufferMap[name]->StoreBufferData(value);
		mVertexShaderConstantBufferMap[name]->SubmitDataToDevice(deviceContext);

		UINT bufferIndex = mVertexShaderConstantBufferMap[name]->GetBufferIndex();

		deviceContext->VSSetConstantBuffers(bufferIndex, 1, (ID3D11Buffer* const*)mVertexShaderConstantBufferMap[name]->GetBufferPointerRef());
	}
	else
	{
		check(false);
	}
}

template<class T>
void SSMaterial::SetPSConstantBufferData(ID3D11DeviceContext* deviceContext, SSName name, const T& value)
{
	if (mPixelShaderConstantBufferMap.count(name) > 0)
	{
		mPixelShaderConstantBufferMap[name]->StoreBufferData(value);
		mPixelShaderConstantBufferMap[name]->SubmitDataToDevice(deviceContext);

		UINT bufferIndex = mPixelShaderConstantBufferMap[name]->GetBufferIndex();

		deviceContext->PSSetConstantBuffers(bufferIndex, 1, mPixelShaderConstantBufferMap[name]->GetBufferPointerRef());
	}
}
