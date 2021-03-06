
#include "SSDX12.h"
#include "SSDX12Shader.h"
#include <filesystem>
#include "SSDXTranslator.h"


void SSVertexShader::CompileFromFile(ID3D12Device* device, std::wstring filepath)
{
	D3D_SHADER_MACRO macro;
	
	ID3DBlob* errorMsg = nullptr;

	
}

void SSVertexShader::CreateInputLayout(ID3D12Device* device, ID3D12ShaderReflection* shaderReflection)
{
	check(device != nullptr);

	D3D12_SHADER_DESC shaderDesc;
	shaderReflection->GetDesc(&shaderDesc);

	for(UINT i = 0; i < shaderDesc.ConstantBuffers; ++i)
	{
		
	}

	const UINT inputParamCount = shaderDesc.InputParameters;

	D3D12_INPUT_ELEMENT_DESC* inputDescriptions = new D3D12_INPUT_ELEMENT_DESC[inputParamCount];
	UINT byteOffset = 0;

	for (UINT i = 0; i < shaderDesc.InputParameters; ++i)
	{
		D3D12_SIGNATURE_PARAMETER_DESC inputDesc;
		shaderReflection->GetInputParameterDesc(i, &inputDesc);

		auto format = SSDXTranslator::GetVertexShaderInputType(inputDesc);
		inputDescriptions[i].SemanticName = inputDesc.SemanticName;
		inputDescriptions[i].Format = format;
		inputDescriptions[i].InputSlotClass = D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		inputDescriptions[i].SemanticIndex = 0;
		inputDescriptions[i].InstanceDataStepRate = 0;
		inputDescriptions[i].InputSlot = 0;
		inputDescriptions[i].AlignedByteOffset = byteOffset;

		byteOffset += SSDXTranslator::GetDXGIFormatByteSize(format);
	}

	
}