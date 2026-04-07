

#pragma once

#include <vector>
#include "SSDX11Buffer.h"
#include "SSDX11Renderer.h"

class SSDX11VertexBuffer : public SSBufferBase
{
public:
    SSDX11VertexBuffer(ID3D11Buffer* ptrBuffer, unsigned int stride, unsigned int count);    
    ~SSDX11VertexBuffer();
    
    template<class T>
    void SetVertexBufferData(const std::vector<T>& vertexData);

    virtual void* GetBufferPointer() override;
    virtual void* const* GetBufferPointerRef() override;

    virtual unsigned int GetStride() const { return mStride; }
    virtual unsigned int GetVertexCount() const { return mVertexCount; }

    virtual BYTE* GetBufferDataPtr() override { return nullptr; }

protected:
    template<class T>
    void InternalCreateVertexBuffer(const std::vector<T>& vertexData);
    
    D3D11_BUFFER_DESC mBufferDescription{};
    ComPtr<ID3D11Buffer> mpBuffer = nullptr;

protected:
    unsigned int mStride = 0;
    unsigned int mVertexCount = 0;
};

template<class T>
void SSDX11VertexBuffer::SetVertexBufferData(const std::vector<T>& vertexData)
{
    if(mpBuffer == nullptr)
    {
        InternalCreateVertexBuffer(vertexData);
    }
}

class SSDX11InstancedVertexBuffer : public SSDX11Buffer
{
public:
    SSDX11InstancedVertexBuffer(){}
    template<class T>
    void SetInstanceVertexBufferData(const std::vector<T>& instanceData);
    UINT GetStride() const { return mStride;}
    UINT GetInstanceCount() const { return mInstanceCount;}
protected:
    UINT mStride = 0;
    UINT mInstanceCount = 0;
};

template<class T>
void SSDX11InstancedVertexBuffer::SetInstanceVertexBufferData(const std::vector<T> &instanceData)
{
    if(mpBuffer==nullptr)
    {
        mBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        mBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        mBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
        mBufferDescription.MiscFlags = 0;
        mBufferDescription.StructureByteStride = 0;
        mBufferDescription.ByteWidth = static_cast<UINT>(sizeof(T) * instanceData.size());

        D3D11_SUBRESOURCE_DATA vertexSubresourceData;

        vertexSubresourceData.pSysMem = &instanceData[0];
        vertexSubresourceData.SysMemPitch = 0;
        vertexSubresourceData.SysMemSlicePitch = 0;

        HR(SSDX11Renderer::Get().GetDevice()->CreateBuffer(&mBufferDescription, &vertexSubresourceData, &mpBuffer));

        mStride = sizeof(T);
        mInstanceCount = static_cast<UINT>(instanceData.size());
    }
}


template<class T>
void SSDX11VertexBuffer::InternalCreateVertexBuffer(const std::vector<T>& vertexData)
{
    mBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    mBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    mBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
    mBufferDescription.MiscFlags = 0;
    mBufferDescription.StructureByteStride = 0;
    mBufferDescription.ByteWidth = static_cast<UINT>(sizeof(T) * vertexData.size());

    D3D11_SUBRESOURCE_DATA vertexSubresourceData;

    vertexSubresourceData.pSysMem = &vertexData[0];
	vertexSubresourceData.SysMemPitch = 0;
	vertexSubresourceData.SysMemSlicePitch = 0;

    HR(SSDX11Renderer::Get().GetDevice()->CreateBuffer(&mBufferDescription, &vertexSubresourceData, &mpBuffer));

	mStride = sizeof(T);
	mVertexCount = static_cast<UINT>(vertexData.size());
}
