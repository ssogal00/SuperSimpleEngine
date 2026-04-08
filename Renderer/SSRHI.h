#pragma once

#include <vector>
#include <memory>
#include "d3d11.h"

#include "wrl/client.h"
using Microsoft::WRL::ComPtr;

class SSDX11IndexBuffer;
class SSDX11VertexBuffer;
class SSDX11ConstantBuffer;
class SSDX11PixelShader;
class SSDX11VertexShader;
class SSDX11Texture2D;


std::shared_ptr<SSDX11IndexBuffer> RHICreateIndexBuffer(std::vector<unsigned short>& InData);
std::shared_ptr<SSDX11IndexBuffer> RHICreateIndexBuffer(std::vector<unsigned int>& InData);
std::shared_ptr<SSDX11VertexBuffer> RHICreateVertexBuffer(unsigned int InStride, unsigned int InCount, void* InPtrData, bool bIsImmutable);
std::shared_ptr<SSDX11ConstantBuffer> RHICreateConstantBuffer(const UINT InBufferSize, const UINT InSlotIndex, std::string InName);
void RHIUpdateConstantBuffer(SSDX11ConstantBuffer* InConstantBuffer, const void* InData, size_t InDataSize);
void RHISetPixelShader(SSDX11PixelShader* InPixelShader);
void RHISetVertexShader(SSDX11VertexShader* InVertexShader);
void RHISetTexture(SSDX11PixelShader* InPixelShader, std::string InSlotName, SSDX11Texture2D* InTexture);
void RHISetTexture(SSDX11VertexShader* InVertexShader, std::string InSlotName, SSDX11Texture2D* InTexture);


