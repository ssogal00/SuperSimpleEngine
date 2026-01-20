

#include "SSDX11ConstantBuffer.h"
#include "SSShader.h"

// trigger compile
SSDX11ConstantBuffer::SSDX11ConstantBuffer(ID3D11Buffer* InBuffer, UINT InSlotIndex, UINT InSize, std::string InName)
{
    mpBuffer = InBuffer;
    mBufferSlotIndex = InSlotIndex;
    mBufferSize = InSize;
    mBufferName = InName;

    // alloc
    mBufferData = new BYTE[mBufferSize]{ 0 };
}

SSDX11ConstantBuffer::SSDX11ConstantBuffer(ID3D11ShaderReflectionConstantBuffer* constantBuffer, UINT index)
{
    mBufferSlotIndex = index;

    check(constantBuffer != nullptr);

    D3D11_SHADER_BUFFER_DESC BufferDesc;
    constantBuffer->GetDesc(&BufferDesc);

	mBufferSize = BufferDesc.Size;
    mBufferName = BufferDesc.Name;
    
    // 
    for(unsigned int i = 0; i < BufferDesc.Variables; ++i)
    {
        ID3D11ShaderReflectionVariable* variableReflection = constantBuffer->GetVariableByIndex(i);
        D3D11_SHADER_VARIABLE_DESC variableDesc;
        variableReflection->GetDesc(&variableDesc);
        
        ID3D11ShaderReflectionType* variableType = variableReflection->GetType();
        D3D11_SHADER_TYPE_DESC typeDesc;
        variableType->GetDesc(&typeDesc);

        VariableInConstantBufferInfo info 
        {
            static_cast<USHORT>(variableDesc.StartOffset),
            static_cast<USHORT>(variableDesc.Size),
            static_cast<BYTE>(i),
            variableDesc.Name
        };

        mVariableInfoArray.push_back(info);
    }

    // alloc
    mBufferData = new BYTE[mBufferSize] {0};

    // now create constant buffer
    mBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    mBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    mBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
    mBufferDescription.MiscFlags = 0;
    mBufferDescription.StructureByteStride = 0;
    mBufferDescription.ByteWidth = mBufferSize;

    HR(SSDX11Renderer::Get().GetDevice()->CreateBuffer(&mBufferDescription, nullptr, mpBuffer.GetAddressOf()));
}

SSDX11ConstantBuffer::~SSDX11ConstantBuffer()
{
    if(mBufferData != nullptr)
    {
        delete [] mBufferData;
        mBufferData = nullptr;
    }
}

void SSDX11ConstantBuffer::SetBufferData(const SSConstantBufferData& data)
{   
    check(mpBuffer != nullptr);
    
    check(data.GetBufferSize() == mBufferSize);

    memcpy_s(mBufferData, mBufferSize, data.GetData(), mBufferSize);
}

bool SSDX11ConstantBuffer::UpdateBufferData(void* InDataPtr, unsigned int InDataLength)
{
    check(mpBuffer != nullptr);

    check(mBufferSize == InDataLength);

    if (memcmp(mBufferData, InDataPtr, InDataLength) == 0)
    {        
        bDirty = false;
        return false;
    }
    
    bDirty = true;
    memcpy_s(mBufferData, mBufferSize, InDataPtr, mBufferSize);

    return true;
}

void SSDX11ConstantBuffer::SetBufferData(void* InDataPtr, unsigned int InDataLength)
{
    check(mpBuffer != nullptr);    

    check(mBufferSize == InDataLength);

    memcpy_s(mBufferData, mBufferSize, InDataPtr, mBufferSize);
}

void SSDX11ConstantBuffer::SubmitDataToDevice(ID3D11DeviceContext* deviceContext)
{ 	
	check(deviceContext);    

	check(mBufferDescription.CPUAccessFlags & D3D11_CPU_ACCESS_WRITE);  
	check(mBufferDescription.Usage == D3D11_USAGE_DYNAMIC);

    D3D11_MAPPED_SUBRESOURCE mappedResource;
	HR(deviceContext->Map(mpBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
    memcpy_s(mappedResource.pData, mBufferSize, mBufferData, mBufferSize);
	deviceContext->Unmap(mpBuffer.Get(), 0);
}
