#pragma once

#include <vector>
#include <memory>
#include "d3d11.h"

#include "wrl/client.h"
using Microsoft::WRL::ComPtr;

class SSDX11IndexBuffer;
class SSDX11VertexBuffer;
class SSDX11ConstantBuffer;

std::shared_ptr<SSDX11IndexBuffer> RHICreateIndexBuffer(std::vector<unsigned short>& InData);
std::shared_ptr<SSDX11IndexBuffer> RHICreateIndexBuffer(std::vector<unsigned int>& InData);
std::shared_ptr<SSDX11VertexBuffer> RHICreateVertexBuffer(unsigned int InStride, unsigned int InCount, void* InPtrData, bool bIsImmutable);
std::shared_ptr<SSDX11ConstantBuffer> RHICreateConstantBuffer(const UINT InBufferSize, const UINT InSlotIndex, std::string InName);