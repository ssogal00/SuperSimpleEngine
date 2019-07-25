#include "Core.h"
#include "SSShader.h"
#include "SSEngine.h"
#include "DXVertexElementDeclaration.h"
#include <filesystem>
#include <fstream>
#include <map>
#include "Templates.h"
#include "Translator.h"

#pragma region DXShaderImplementation

void SSShader::PrintCompileError(ID3D10Blob* errorMessage)
{	
    auto buffSize = errorMessage->GetBufferSize();

    const char* errorMsgPtr = (const char*) errorMessage->GetBufferPointer();    

	char* compileErrBuffer = new char[buffSize + 1];

	strcpy_s(compileErrBuffer, buffSize, errorMsgPtr);

    OutputDebugStringA("============= Shader Compile Error =============\n");
	OutputDebugStringA(compileErrBuffer);
    OutputDebugStringA("============= Shader Compile Error =============\n");

	delete[] compileErrBuffer;
}

ID3D11Buffer* SSShader::GetConstantBuffer(std::string bufferName)
{
    if(mConstantBufferMap.count(bufferName) > 0)
    {
        return mConstantBufferMap[bufferName]->GetBufferPointer();
    }

    return nullptr;
}

void SSShader::ReflectCompiledShader(ID3D11ShaderReflection* shaderReflection)
{
	assert(mShaderBuffer != nullptr);
	assert(shaderReflection != nullptr);	

	D3D11_SHADER_DESC shaderDescription;
	shaderReflection->GetDesc(&shaderDescription);

	for (unsigned int i = 0; i < shaderDescription.ConstantBuffers; ++i)
	{
		ID3D11ShaderReflectionConstantBuffer* constantBuffer = shaderReflection->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC bufferDesc;
		constantBuffer->GetDesc(&bufferDesc);

		mConstantBufferMap[bufferDesc.Name] = new DXGenericConstantBuffer(constantBuffer, i);
	}
	// @ end

	//
	for (unsigned int i = 0; i < shaderDescription.BoundResources; ++i)
	{
		D3D11_SHADER_INPUT_BIND_DESC desc;
		shaderReflection->GetResourceBindingDesc(i, &desc);

		if (desc.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_TEXTURE)
		{
			mTextureMap[desc.Name] = desc.BindPoint;
		}
	}
}

#pragma endregion


//@ vertex shader implementation
 SSVertexShader::~SSVertexShader()
 {
     ReleaseCOM(mVertexShader);
 }


void SSVertexShader::CreateInputLayout(ID3D11ShaderReflection* shaderReflection)
{
    auto* dxDevice = SSEngine::Get().GetDevice();
    
    check(dxDevice != nullptr);    
	check(shaderReflection != nullptr);

	D3D11_SHADER_DESC shaderDescription;
	shaderReflection->GetDesc(&shaderDescription);

	for (unsigned int i = 0; i < shaderDescription.ConstantBuffers; ++i)
	{
		ID3D11ShaderReflectionConstantBuffer* constantBuffer = shaderReflection->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC bufferDesc;
		constantBuffer->GetDesc(&bufferDesc);

		mConstantBufferMap[bufferDesc.Name] = new DXGenericConstantBuffer(constantBuffer, i);
	}

	// @input layout creation
	UINT inputParamCount = shaderDescription.InputParameters;
	D3D11_INPUT_ELEMENT_DESC* inputDescriptions = new D3D11_INPUT_ELEMENT_DESC[inputParamCount];
	UINT byteOffset = 0;

	for (UINT i = 0; i < shaderDescription.InputParameters; ++i)
	{
		auto* dxDevice = SSEngine::Get().GetDevice();

		check(dxDevice != nullptr);

		D3D11_SIGNATURE_PARAMETER_DESC inputDesc;
		shaderReflection->GetInputParameterDesc(i, &inputDesc);

		auto format = Translator::GetVertexShaderInputType(inputDesc);
		inputDescriptions[i].SemanticName = inputDesc.SemanticName;
		inputDescriptions[i].Format = format;
		inputDescriptions[i].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
		inputDescriptions[i].SemanticIndex = 0;
		inputDescriptions[i].InstanceDataStepRate = 0;
		inputDescriptions[i].InputSlot = 0;
		inputDescriptions[i].AlignedByteOffset = byteOffset;

		byteOffset += Translator::GetDXGIFormatByteSize(format);
	}

	HR(dxDevice->CreateInputLayout(inputDescriptions, inputParamCount, mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), &mInputLayout));
    
}

 bool SSVertexShader::CompileFromFile(std::wstring filepath)
 {
    ID3D10Blob* errorMsg = nullptr;

    check(std::filesystem::exists(filepath));

    D3DCompileFromFile(filepath.c_str(), nullptr, nullptr, "VSMain", "vs_5_0", 0, 0, &mShaderBuffer, &errorMsg);

    if(errorMsg != nullptr)
    {
		PrintCompileError(errorMsg);
    	
		ReleaseCOM(errorMsg);

        return false;
    }

    auto* dxDevice = SSEngine::Get().GetDevice();
    
	HR(dxDevice->CreateVertexShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mVertexShader));

    
    ID3D11ShaderReflection* vertexShaderReflection = nullptr;    
    HR(D3DReflect(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**) &vertexShaderReflection));  
	
	// @constant buffer reflection
	ReflectCompiledShader(vertexShaderReflection);
    
	// @input layout creation
	CreateInputLayout(vertexShaderReflection);	

    return true;
 }


#pragma region PixelShader
SSPixelShader::~SSPixelShader()
{
    ReleaseCOM(mPixelShader);
}

bool SSPixelShader::CompileFromFile(std::wstring filepath)
{    
    ID3D10Blob* errorMsg = nullptr;

    check(std::filesystem::exists(filepath));

    D3DCompileFromFile(filepath.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, &mShaderBuffer, &errorMsg);

    if(errorMsg != nullptr)
    {
        PrintCompileError(errorMsg);
        return false;
    }

    auto* dxDevice = SSEngine::Get().GetDevice();
    HR(dxDevice->CreatePixelShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mPixelShader));

	// @constant buffer reflection
	ID3D11ShaderReflection* pixelShaderReflection = nullptr;
	HR(D3DReflect(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pixelShaderReflection));

	ReflectCompiledShader(pixelShaderReflection);

    return true;
}
#pragma endregion