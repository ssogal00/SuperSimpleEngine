
#include "Core.h"
#include "SSScreenBlit.h"
#include "SSVertexTypes.h"
#include <vector>
#include "SSDX11VertexBuffer.h"
#include "SSIndexBuffer.h"
#include "SSSharedBufferCache.h"

SSScreenBlit::SSScreenBlit()
{
	mVisible = false;

	mScreenVB = SSSharedBufferCache::Get().GetScreenBlitVertexBuffer();
	mScreenIB = SSSharedBufferCache::Get().GetScreenIndexBuffer();
}

void SSScreenBlit::Destroy()
{	
}

void SSScreenBlit::Draw(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	auto stride = mScreenVB->GetStride();
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, mScreenVB->GetDX11BufferPointerRef(), &stride, &offset);
	deviceContext->IASetIndexBuffer(mScreenIB->GetDX11BufferPointer(), DXGI_FORMAT_R32_UINT, 0);

	deviceContext->DrawIndexed(mScreenIB->GetIndexCount(), 0, 0);
}
