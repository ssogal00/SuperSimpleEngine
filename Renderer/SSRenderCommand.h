
#include <memory>
#include "SSConstantBufferData.h"

class SSRenderCmdBase
{
public:
	virtual void Execute(ID3D11DeviceContext*) = 0;
	virtual ~SSRenderCmdBase();
};

class SSSetPrimivitiveTopology : public SSRenderCmdBase
{
	public:
		SSSetPrimivitiveTopology(D3D11_PRIMITIVE_TOPOLOGY inTopology)
		:mTopology(inTopology)
	{
	}
	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
private:
	D3D11_PRIMITIVE_TOPOLOGY mTopology;
};

// set current vertex shader
class SSRenderCmdSetVS : public SSRenderCmdBase
{
public:
	SSRenderCmdSetVS(std::shared_ptr<class SSDX11VertexShader> inVS)
		:mVS(inVS)
	{
	}

	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;

	void* operator new (size_t size);

	void operator delete(void* Ptr);

private:
	std::shared_ptr<SSDX11VertexShader> mVS;
};

class SSRenderCmdMarker : public SSRenderCmdBase
{
public:
	SSRenderCmdMarker(const std::string& inMarkerName)
		:mMarkerName(inMarkerName)
	{
	}
	std::string mMarkerName;
};

// set current pixel shader
class SSRenderCmdSetPS : public SSRenderCmdBase
{
public:
	SSRenderCmdSetPS(std::shared_ptr<class SSDX11PixelShader> inPS)
		:mPS(inPS)
	{
	}

	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;

private:
	std::shared_ptr<SSDX11PixelShader> mPS;
};
// set vertex shader texture
class SSRenderCmdSetVSTexture : public SSRenderCmdBase
{
public:
	SSRenderCmdSetVSTexture(class SSDX11VertexShader* inVS, class SSDX11Texture2D* inTex, unsigned int slotIndex);

	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;

protected:
	SSDX11VertexShader* mVS;
	SSDX11Texture2D* mTex;
	unsigned int mSlotIndex;
};
// set pixel shader texture
class SSRenderCmdSetPSTexture : public SSRenderCmdBase
{
public:
	SSRenderCmdSetPSTexture(class SSDX11PixelShader* inPS, class SSDX11Texture2D* inTex, unsigned int slotIndex);

	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;

protected:
	SSDX11PixelShader* mPS;
	SSDX11Texture2D* mTex;
	unsigned int mSlotIndex;
};
// set vertex shader constant buffer
class SSRenderCmdSetVSConstantBuffer : public SSRenderCmdBase
{
public:
	SSRenderCmdSetVSConstantBuffer(SSDX11VertexShader* inVS, class SSDX11Buffer* inBuffer, unsigned int slot);
	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
protected:
	SSDX11VertexShader* mVS;
	SSDX11Buffer* mBuffer;
	unsigned int mSlotIndex;
};

class SSRenderCmdSetPSConstantBuffer : public SSRenderCmdBase
{
public:
	SSRenderCmdSetPSConstantBuffer(SSDX11PixelShader* inVS, class SSDX11Buffer* inBuffer, unsigned int slot);
	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
protected:
	SSDX11PixelShader* mPS;
	SSDX11Buffer* mBuffer;
	unsigned int mSlotIndex;
};

class SSRenderCmdUpdateConstantBuffer : public SSRenderCmdBase
{
public:
	SSRenderCmdUpdateConstantBuffer(SSDX11Buffer* ptrBuffer, std::string InBufferName, SSConstantBufferData* InBufferData);
	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
protected:
	SSDX11Buffer* mBuffer;
	std::string mBufferName;
	SSConstantBufferData* mBufferData=nullptr;
};

class SSRenderCmdSetVSShaderResource : public SSRenderCmdBase
{
public:
	SSRenderCmdSetVSShaderResource(class SSDX11StructuredBuffer* inBuffer, unsigned int slotIndex);
protected:
	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;

	SSDX11StructuredBuffer* mBuffer;

	unsigned int mSlotIndex;
};


class SSRenderCmdSetRenderTarget : public SSRenderCmdBase
{
public:
	SSRenderCmdSetRenderTarget(class SSDX11RenderTarget* inRT);
	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
private:
	SSDX11RenderTarget* mRenderTarget;
};
// set vertex buffer
class SSRenderCmdSetVertexBuffer : public SSRenderCmdBase
{
public:
	SSRenderCmdSetVertexBuffer(std::shared_ptr<class SSDX11VertexBuffer> inVB, unsigned int inOffset = 0);
	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
private:
	std::shared_ptr<SSDX11VertexBuffer> mVertexBuffer;
	unsigned int mOffset=0;
};
// set vertex and instanced vertex buffer
class SSRenderCmdSetInstacedVertexBuffer : public SSRenderCmdBase
{
public:
	SSRenderCmdSetInstacedVertexBuffer(
		std::shared_ptr<class SSDX11VertexBuffer> inVB,
		std::shared_ptr<class SSDX11VertexBuffer> inInstanceVB);

	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
private:
	std::shared_ptr<SSDX11VertexBuffer> mVertexBuffer;
	std::shared_ptr<SSDX11VertexBuffer> mInstanceBuffer;
};

// set index buffer
class SSRenderCmdSetIndexBuffer : public SSRenderCmdBase
{
public:
	SSRenderCmdSetIndexBuffer(class std::shared_ptr<class SSDX11IndexBuffer> inIB, unsigned int inOffset = 0);
	virtual void Execute(ID3D11DeviceContext* inDeviceContext)override;
private:
	std::shared_ptr<SSDX11IndexBuffer> mIndexBuffer;
	unsigned int mOffset = 0;
};


// draw with index buffer
class SSRenderCmdDrawIndexed : public SSRenderCmdBase
{
public:
	SSRenderCmdDrawIndexed(class std::shared_ptr<SSDX11IndexBuffer> inBuffer, unsigned int InIndexCount, unsigned int InStartIndexLocation = 0, int InBaseVertexLocation = 0);
	SSRenderCmdDrawIndexed(class std::shared_ptr<SSDX11IndexBuffer> inBuffer);
	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
private:
	std::shared_ptr<SSDX11IndexBuffer> mIndexBuffer;
	unsigned int mIndexCount = 0;
	unsigned int mStartIndexLocation = 0;
	int mBaseVertexLocation = 0;
};

// draw without index buffer
class SSRenderCmdDrawWithoutIndex :public SSRenderCmdBase
{
public:
	SSRenderCmdDrawWithoutIndex(unsigned int InVertexCount);
	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
private:
	unsigned int mVertexCount = 0;
};

// without index but instanced
class SSRenderCmdDrawWithoutIndexInstanced : public SSRenderCmdBase
{
public:
	SSRenderCmdDrawWithoutIndexInstanced(std::shared_ptr<class SSDX11VertexBuffer> inInstanceVB,
		unsigned int InVertexCount,
		unsigned int InInstanceCount);

	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
private:
	std::shared_ptr<SSDX11VertexBuffer> mInstanceBuffer;
	unsigned int mInstanceCount = 0;
	unsigned int mVertexCount = 0;
};


// with index instanced
class SSRenderCmdDrawIndexedInstanced : public SSRenderCmdBase
{
public:
	SSRenderCmdDrawIndexedInstanced(class std::shared_ptr<SSDX11IndexBuffer> inBuffer, unsigned int InInstanceCount);
	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
private:
	std::shared_ptr<SSDX11IndexBuffer> mIndexBuffer;
	
	unsigned int mInstanceCount = 0;
};

class SSRenderCmdClearRenderTarget : public SSRenderCmdBase
{
public:
	
};

class SSRenderCmdSetDepthStencilState : public SSRenderCmdBase
{
public:
	SSRenderCmdSetDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& InDesc)
		: mDepthStencilDesc(InDesc)
	{
	}

	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
protected:
	D3D11_DEPTH_STENCIL_DESC mDepthStencilDesc{};
};


class SSRenderCmdSetViewport : public SSRenderCmdBase
{
public:
	
};