
#pragma once

#include "SSDX11Buffer.h"
#include <vector>
#include <string>
#include "SSConatantBufferData.h"
#include "SSDX11Renderer.h"


/////////////////////////////////////

struct VariableInConstantBufferInfo
{
    USHORT StartOffset;
    USHORT Size;
    BYTE Index;
    std::string Name;
};
// cbuffer in HLSL
class SSDX11ConstantBuffer : public SSDX11Buffer
{
public:
    SSDX11ConstantBuffer(ID3D11ShaderReflectionConstantBuffer* constantBuffer, UINT index);

    SSDX11ConstantBuffer(ID3D11Buffer* InBuffer, UINT InSlotIndex, UINT InSize, std::string InName);

    virtual ~SSDX11ConstantBuffer() override;
        
    void SetBufferData(const SSConstantBufferData& data);

	bool UpdateBufferData(void* InDataPtr, unsigned int InDataLength);
    void SetBufferData(void* InDataPtr, unsigned int InDataLength);

    virtual BYTE* GetBufferDataPtr() override { return mBufferData; }

    virtual void SubmitDataToDevice(ID3D11DeviceContext* deviceContext) override;
protected:
    BYTE* mBufferData = nullptr;

    std::vector<VariableInConstantBufferInfo> mVariableInfoArray;    

	friend class SSDrawCommand;
	friend class SSMaterial;

    
};

