#include "SSRendererModulePCH.h"
#include "SSDX11Renderer.h"
#include "SSDX11IndexBuffer.h"

SSDX11IndexBuffer::SSDX11IndexBuffer(ID3D11Buffer* ptrBuffer, unsigned int nIndexCount)
    : mpBuffer(ptrBuffer)
{
    mIndexCount = nIndexCount;
    ZeroMemory(&mBufferDescription, sizeof(mBufferDescription));
}

void* SSDX11IndexBuffer::GetBufferPointer()
{
    return mpBuffer.Get();
}

void* const* SSDX11IndexBuffer::GetBufferPointerRef()
{
    return (void* const*)mpBuffer.GetAddressOf();
}

SSDX11IndexBuffer::~SSDX11IndexBuffer()
{
    if (mpBuffer)
    {
        mpBuffer.Reset();
    }
}
