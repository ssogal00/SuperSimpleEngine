
#include "SSCustomRenderThreadObject.h"
#include "SSSharedBufferCache.h"
#include "SSDX11VertexBuffer.h"
#include "SSDX11IndexBuffer.h"

SSRenderTargetCube::SSRenderTargetCube()
{
	mVB = SSSharedBufferCache::Get().GetCubeVertexBuffer();
	mIB = SSSharedBufferCache::Get().GetCubeIndexBuffer();
}

void SSRenderTargetCube::Draw(ID3D11DeviceContext *deviceContext)
{
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    auto stride = mVB->GetStride();
    UINT offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, (ID3D11Buffer* const*) mVB->GetBufferPointerRef(), &stride, &offset);
    deviceContext->IASetIndexBuffer( (ID3D11Buffer*)mIB->GetBufferPointer(), DXGI_FORMAT_R32_UINT, 0);

    deviceContext->DrawIndexed(mIB->GetIndexCount(), 0, 0);
}


void SSCubeMapRenderingSphere::Draw(ID3D11DeviceContext* deviceContext)
{
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    auto stride = mVB->GetStride();
    UINT offset = 0;
    deviceContext->IASetVertexBuffers(0,1,(ID3D11Buffer* const*) mVB->GetBufferPointerRef(), &stride, &offset);
    deviceContext->Draw(mVB->GetVertexCount(), 0);
}

SSCubeMapRenderingSphere::SSCubeMapRenderingSphere()
{
    mVB = SSSharedBufferCache::Get().GetSphereVertexBuffer();
}
