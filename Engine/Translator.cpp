
#include "Core.h"
#include "Translator.h"

UINT Translator::GetDXGIFormatByteSize(DXGI_FORMAT format)
{
	switch(format)
	{
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
		return sizeof(float) * 4;
	case DXGI_FORMAT_R32G32B32_FLOAT:
		return sizeof(float) * 3;
	case DXGI_FORMAT_R32G32_FLOAT:
		return sizeof(float) * 2;
	case DXGI_FORMAT_R32_FLOAT:
		return sizeof(float);

	case DXGI_FORMAT_R32G32B32A32_UINT:
		return sizeof(UINT) * 4;
	case DXGI_FORMAT_R32G32B32_UINT:
		return sizeof(UINT) * 3;
	case DXGI_FORMAT_R32G32_UINT:
		return sizeof(UINT) * 2;
	case DXGI_FORMAT_R32_UINT:
		return sizeof(UINT);

	
	}

	return 0;
}


DXGI_FORMAT Translator::GetVertexShaderInputType(D3D11_SIGNATURE_PARAMETER_DESC desc)
{
    unsigned char nComponentCount = 0;
    unsigned char nMask = desc.Mask;

    while((nMask & 0x01) != 0)
    {
        nComponentCount++;
        nMask = nMask >> 1;        
    }

    if(desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
    {
        switch(nComponentCount)
        {
            case 4:
                return DXGI_FORMAT_R32G32B32A32_FLOAT;
                break;
            case 3:
                return DXGI_FORMAT_R32G32B32_FLOAT;
                break;
            case 2:
                return DXGI_FORMAT_R32G32_FLOAT;
                break;
            case 1:
                return DXGI_FORMAT_R32_FLOAT;
                break;
        }                
    }

    else if(desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
    {
        switch(nComponentCount)
        {
            case 4:
                return DXGI_FORMAT_R32G32B32A32_UINT;
                break;
            case 3:
                return DXGI_FORMAT_R32G32B32_UINT;
                break;
            case 2:
                return DXGI_FORMAT_R32G32_UINT;
                break;
            case 1:
                return DXGI_FORMAT_R32_UINT;
                break;
        }                
    }

    else if(desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
    {
        switch(nComponentCount)
        {
            case 4:
                return DXGI_FORMAT_R32G32B32A32_SINT;
                break;
            case 3:
                return DXGI_FORMAT_R32G32B32_SINT;
                break;
            case 2:
                return DXGI_FORMAT_R32G32_SINT;
                break;
            case 1:
                return DXGI_FORMAT_R32_SINT;
                break;
        }                
    }


    return DXGI_FORMAT_R32_FLOAT;
}

