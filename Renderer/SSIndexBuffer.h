#pragma once
#include <d3dcommon.h>
#include "SSBufferBase.h"
#include <vector>

class SSIndexBuffer : public SSBufferBase
{
public:
	D3D_PRIMITIVE_TOPOLOGY GetPrimitiveType() const { return mPrimitiveType; }
	UINT GetIndexCount() { return mIndexCount; }
protected:
	D3D_PRIMITIVE_TOPOLOGY mPrimitiveType = D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT mIndexCount = 0;
};
