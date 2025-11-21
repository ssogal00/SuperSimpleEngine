
#include <memory>

class SSRenderCmdBase
{
public:
	virtual void Execute(ID3D11DeviceContext*) = 0;
	virtual ~SSRenderCmdBase();
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
class SSRenderCmdSetVSCBuffer : public SSRenderCmdBase
{
public:
	SSRenderCmdSetVSCBuffer(SSDX11VertexShader* inVS, class SSDX11Buffer* inBuffer, unsigned int slot);
	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
protected:
	SSDX11VertexShader* mVS;
	SSDX11Buffer* mBuffer;
	unsigned int mSlotIndex;
};

class SSRenderCmdSetPSCBuffer : public SSRenderCmdBase
{
public:
	SSRenderCmdSetPSCBuffer(SSDX11PixelShader* inVS, class SSDX11Buffer* inBuffer, unsigned int slot);
	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
protected:
	SSDX11PixelShader* mPS;
	SSDX11Buffer* mBuffer;
	unsigned int mSlotIndex;
};

class SSRenderCmdCopyCBuffer : public SSRenderCmdBase
{
public:
	SSRenderCmdCopyCBuffer(SSDX11Buffer* ptrBuffer);
	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
protected:
	SSDX11Buffer* mBuffer;
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
	SSRenderCmdSetVertexBuffer(std::shared_ptr<class SSDX11VertexBuffer> inVB);
	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
private:
	std::shared_ptr<SSDX11VertexBuffer> mVertexBuffer;
};
// set index buffer
class SSRenderCmdSetIndexBuffer : public SSRenderCmdBase
{
public:
	SSRenderCmdSetIndexBuffer(class std::shared_ptr<class SSDX11IndexBuffer> inIB);
	virtual void Execute(ID3D11DeviceContext* inDeviceContext)override;
private:
	std::shared_ptr<SSDX11IndexBuffer> mIndexBuffer;
};

class SSRenderCmdDrawIndexed : public SSRenderCmdBase
{
public:
	SSRenderCmdDrawIndexed(class std::shared_ptr<SSDX11IndexBuffer> inBuffer);
	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
private:
	std::shared_ptr<SSDX11IndexBuffer> mIndexBuffer;
};

class SSRenderCmdDrawWithoutIndex :public SSRenderCmdBase
{
public:
	SSRenderCmdDrawWithoutIndex(unsigned int InVertexCount);
	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
private:
	unsigned int mVertexCount = 0;
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