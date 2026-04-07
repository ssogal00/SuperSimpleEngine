
#include "SSRendererModulePCH.h"
#include "SSDX11VertexBuffer.h"

SSDX11VertexBuffer::SSDX11VertexBuffer(ID3D11Buffer* ptrBuffer, unsigned int stride, unsigned int count)
	:	mpBuffer(ptrBuffer),
		mStride(stride),
		mVertexCount(count)
{	
}

void* SSDX11VertexBuffer::GetBufferPointer()
{
	return (void*) mpBuffer.Get();
}
void* const* SSDX11VertexBuffer::GetBufferPointerRef()
{
	return (void* const*)mpBuffer.GetAddressOf();
}


SSDX11VertexBuffer::~SSDX11VertexBuffer()
{
	if(mpBuffer)
	{
		mpBuffer.Reset();
	}
}