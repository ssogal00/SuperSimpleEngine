
#include "SSRendererModulePCH.h"
#include "SSDX11ConstantBuffer.h"
#include "SSObjectBase.h"
#include "SSName.h"
#include <functional>
#include <map>

class SSDX11VertexShader;
class SSDX11PixelShader;
class SSGameObject;
class SSDX11Texture2D;
class IRenderTarget;


// 
// 1. vertex shader
// 
class SSDrawObjectCommand
{
public:
	// draw하기 위해 필요한 것들
	// 1. vertex , pixel shader
	// 2. constant buffer data (slot, size)
	// 3. texture
	// 4. sampler
	// 5. primitive type
	// 6. vertex buffer, index buffer 
	// 7. vertex buffer stride, offset 
	// if any changes in rasterize state, depth state, blend state
	// if any changes in render target
public:
	virtual void Do(SSDX11Device* device);
private:
	ID3D11VertexShader* mVertexShader = nullptr;
	ID3D11PixelShader* mPixelShader = nullptr;
	ID3D11Buffer* mVertexConstantBuffer = nullptr;
	ID3D11Buffer* mPixelConstantBuffer = nullptr;
	ID3D11Buffer* mVertexBuffer = nullptr;	
	ID3D11Buffer* mIndexBuffer = nullptr;
	ID3D11InputLayout* mInputLayout = nullptr;
	ID3D11ShaderResourceView* mPixelShaderTextureView = nullptr;
	ID3D11ShaderResourceView* mVertexShaderTextureView = nullptr;
	UINT mVertexStride = 0;
	UINT mVertexOffset = 0;
};

class SSDrawCmdBase
{
public :	
	virtual void Do(SSDX11Device* deviceContext) = 0;
};


class SSDrawCommand  : public SSDrawCmdBase
{
public:
	//SSDrawCommand(SSVertexShader* vs, SSPixelShader* ps, std::shared_ptr<SSObjectBase> object);
	SSDrawCommand(std::shared_ptr<SSDX11VertexShader> vs, std::shared_ptr<SSDX11PixelShader> ps, std::shared_ptr<SSObjectBase> object);

	template<class T>
	void StoreVSConstantBufferData(std::string name, const T& value);

	template<class T>
	void StorePSConstantBufferData(std::string name, const T& value);

	void SetPSTexture(std::string name, SSDX11Texture2D* texture);

	void SetVSTexture(std::string name, SSDX11Texture2D* texture);

	void SetPrimitiveType(D3D_PRIMITIVE_TOPOLOGY ePrimitiveType) { mPrimitiveType = ePrimitiveType; }

	D3D_PRIMITIVE_TOPOLOGY GetPrimitiveType() const { return mPrimitiveType; }

	virtual void Do(SSDX11Device* deviceContext) override;

protected:
	std::shared_ptr<class SSDX11VertexShader> mpVS = nullptr;
	std::shared_ptr<class SSDX11PixelShader> mpPS = nullptr;
	class SSMaterial* mMaterial = nullptr;	

	//
	std::map<std::string, class SSDX11ConstantBuffer*> mVertexShaderConstantBufferMap;
	std::map<std::string, class SSDX11ConstantBuffer*> mPixelShaderConstantBufferMap;

	//
	std::map<std::string, class SSDX11Texture2D*> mPixelShaderTextureMap;
	std::map<std::string, class SSDX11Texture2D*> mVertexShaderTextureMap;

	//
	D3D_PRIMITIVE_TOPOLOGY mPrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	
	std::shared_ptr<SSObjectBase> mObject;
};

template<class T>
void SSDrawCommand::StoreVSConstantBufferData(std::string name, const T& value)
{
	if (mVertexShaderConstantBufferMap.count(name) > 0)
	{
		mVertexShaderConstantBufferMap[name]->SetBufferData((void*) &value, sizeof(T));
	}
}

template<class T>
void SSDrawCommand::StorePSConstantBufferData(std::string name, const T& value)
{
	if (mPixelShaderConstantBufferMap.count(name) > 0)
	{
		mPixelShaderConstantBufferMap[name]->SetBufferData((void*)&value, sizeof(T));
	}
}
