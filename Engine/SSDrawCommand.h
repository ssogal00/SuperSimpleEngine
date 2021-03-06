
#include "Core.h"
#include "DXConstantBuffer.h"
#include "SSRenderThreadObject.h"
#include "SSName.h"
#include <functional>
#include <map>

class SSVertexShader;
class SSPixelShader;
class SSGameObject;
class SSTexture2DBase;
class IRenderTarget;


class ENGINE_API SSDrawCmdBase : public SSRenderThreadObject
{
public :	
	virtual void Do(ID3D11DeviceContext* deviceContext) = 0;
};


class ENGINE_API SSChangeRenderTargetCmd : public SSDrawCmdBase
{
public:
	SSChangeRenderTargetCmd(IRenderTarget* renderTarget);
	
	virtual void Do(ID3D11DeviceContext* deviceContext) override;
protected:
	IRenderTarget* mRenderTarget = nullptr;
};

class ENGINE_API SSDepthStateChangeCmd: public SSDrawCmdBase
{
public:
	SSDepthStateChangeCmd();

protected:

};


class ENGINE_API SSDrawCommand  : public SSDrawCmdBase
{
public:
	SSDrawCommand(SSVertexShader* vs, SSPixelShader* ps, std::shared_ptr<SSGameObject> object);

	SSDrawCommand(SSMaterial* material, std::shared_ptr<SSGameObject> object);

	template<class T>
	void StoreVSConstantBufferData(SSName name, const T& value);

	template<class T>
	void StorePSConstantBufferData(SSName name, const T& value);

	void SetPSTexture(std::string name, SSTexture2DBase* texture);

	void SetVSTexture(std::string name, SSTexture2DBase* texture);	

	void SetPrimitiveType(D3D_PRIMITIVE_TOPOLOGY ePrimitiveType) { mPrimitiveType = ePrimitiveType; }

	D3D_PRIMITIVE_TOPOLOGY GetPrimitiveType() const { return mPrimitiveType; }

	virtual void Do(ID3D11DeviceContext* deviceContext) override;

protected:
	class SSVertexShader* mpVS = nullptr;
	class SSPixelShader* mpPS = nullptr;	
	class SSMaterial* mMaterial = nullptr;

	class SSVertexBuffer* mpVertexBuffer = nullptr;
	class SSIndexBuffer* mpIndexBuffer = nullptr;	

	//
	std::map<SSName, class SSGenericConstantBuffer*> mVertexShaderConstantBufferMap;
	std::map<SSName, class SSGenericConstantBuffer*> mPixelShaderConstantBufferMap;

	//
	std::map<std::string, class SSTexture2DBase*> mPixelShaderTextureMap;
	std::map<std::string, class SSTexture2DBase*> mVertexShaderTextureMap;

	//
	D3D_PRIMITIVE_TOPOLOGY mPrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	std::shared_ptr<SSGameObject> mObject;

	std::function<void()> mPreDrawJob = nullptr;
	std::function<void()> mPostDrawJob = nullptr;

};

template<class T>
void SSDrawCommand::StoreVSConstantBufferData(SSName name, const T& value)
{
	if (mVertexShaderConstantBufferMap.count(name) > 0)
	{
		mVertexShaderConstantBufferMap[name]->StoreBufferData(value);		
	}
}

template<class T>
void SSDrawCommand::StorePSConstantBufferData(SSName name, const T& value)
{
	if (mPixelShaderConstantBufferMap.count(name) > 0)
	{
		mPixelShaderConstantBufferMap[name]->StoreBufferData(value);		
	}
}
