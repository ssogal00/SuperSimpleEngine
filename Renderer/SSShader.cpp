#include "SSRendererModulePCH.h"
#include "SSShader.h"
#include "SSDX11Renderer.h"
#include <filesystem>
#include <map>
#include "SSDXTranslator.h"
#include "SSDX11Texture2D.h"
#include "d3d11shader.h"
#pragma region DXShaderImplementation

void SSShader::PrintCompileError(ID3DBlob* errorMessage)
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

SSDX11ConstantBuffer* SSShader::GetConstantBuffer(std::string bufferName)
{
    if(mConstantBufferMap.count(bufferName) > 0)
    {
        return mConstantBufferMap[bufferName];
    }

    return nullptr;
}

void SSShader::ReflectCompiledShader(ID3D11ShaderReflection* shaderReflection)
{
	check(mShaderBuffer != nullptr);
	check(shaderReflection != nullptr);
	
	D3D11_SHADER_DESC shaderDescription;
	HR(shaderReflection->GetDesc(&shaderDescription));

	mShaderType = static_cast<D3D11_SHADER_VERSION_TYPE>(D3D11_SHVER_GET_TYPE(shaderDescription.Version));
	check(mShaderType != D3D11_SHADER_VERSION_TYPE::D3D11_SHVER_RESERVED0);

	//
	for (unsigned int i = 0; i < shaderDescription.BoundResources; ++i)
	{
		D3D11_SHADER_INPUT_BIND_DESC desc;
		shaderReflection->GetResourceBindingDesc(i, &desc);

		if (desc.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_TEXTURE)
		{
			mTextureMap[desc.Name] = desc.BindPoint;
		}
		else if (desc.Type == D3D_SHADER_INPUT_TYPE::D3D10_SIT_SAMPLER)
		{
			mSamplerMap[desc.Name] = desc.BindPoint;
		}
		else if(desc.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_STRUCTURED)
		{
		
		}
		else if(desc.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_UAV_RWSTRUCTURED)
		{
		}
		else if(desc.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_CBUFFER)
		{
			// constant buffer
		}
		else
		{
			// other types
		}
	}
	
	for (unsigned int i = 0; i < shaderDescription.ConstantBuffers; ++i)
	{		
		ID3D11ShaderReflectionConstantBuffer* constantBuffer = shaderReflection->GetConstantBufferByIndex(i);
		
		D3D11_SHADER_BUFFER_DESC bufferDesc;
		constantBuffer->GetDesc(&bufferDesc);
		if(bufferDesc.Type == D3D_CBUFFER_TYPE::D3D_CT_CBUFFER)
		{
			// regular constant buffer
			mConstantBufferMap[bufferDesc.Name] = GetDX11Device()->CreateConstantBuffer(bufferDesc.Size, i, bufferDesc.Name);
		}
		else
		{
		}
	}
	// @ end

	
}

std::vector<std::string> SSShader::GetSamplerNames() 
{
	std::vector<std::string> result;
	for (auto& k : mSamplerMap)
	{
		result.push_back(k.first);
	}
	return result;
}


int SSShader::GetConstantBufferSlotIndex(const std::string& InName)
{
	if (mConstantBufferMap.count(InName)>0)
	{
		return mConstantBufferMap[InName]->GetBufferIndex();
	}

	return -1;
}


int SSShader::GetTextureSlotIndex(const std::string& InName)
{
	if (mTextureMap.count(InName)>0)
	{
		return mTextureMap[InName];
	}

	return -1;
}

int SSShader::GetSamplerSlotIndex(const std::string& InName)
{
	if (mSamplerMap.count(InName)>0)
	{
		return mSamplerMap[InName];
	}

	return -1;
}


#pragma endregion


SSDX11VertexShader::~SSDX11VertexShader()
{
	for (auto& kvp : mConstantBufferMap)
	{
		if (kvp.second)
		{
			//kvp.second->Destroy();
		}
	}

	ReleaseCOM(mVertexShader);
}

void SSDX11VertexShader::CreateInputLayout(ID3D11ShaderReflection* shaderReflection, const SSCompileContext& context)
{
    auto* dxDevice = SSDX11Renderer::Get().GetDevice();
    
    check(dxDevice != nullptr);    
	check(shaderReflection != nullptr);

	D3D11_SHADER_DESC shaderDescription;
	shaderReflection->GetDesc(&shaderDescription);

	for (unsigned int i = 0; i < shaderDescription.ConstantBuffers; ++i)
	{
		ID3D11ShaderReflectionConstantBuffer* constantBuffer = shaderReflection->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC bufferDesc;
		constantBuffer->GetDesc(&bufferDesc);

		mConstantBufferMap[bufferDesc.Name] = new SSDX11ConstantBuffer(constantBuffer, i);
	}

	// @input layout creation
	UINT inputParamCount = shaderDescription.InputParameters;
	D3D11_INPUT_ELEMENT_DESC* inputDescriptions = new D3D11_INPUT_ELEMENT_DESC[inputParamCount];
	UINT byteOffset = 0;

	map<std::string, int> semanticIndexMap;

	for (UINT i = 0; i < shaderDescription.InputParameters; ++i)
	{
		auto* dxDevice = SSDX11Renderer::Get().GetDevice();

		check(dxDevice != nullptr);

		D3D11_SIGNATURE_PARAMETER_DESC inputDesc;
		shaderReflection->GetInputParameterDesc(i, &inputDesc);

		auto format = SSDXTranslator::GetVertexShaderInputType(inputDesc);
		inputDescriptions[i].SemanticName = inputDesc.SemanticName;
		inputDescriptions[i].Format = format;

		if(semanticIndexMap.count(inputDesc.SemanticName) > 0)
        {
		    semanticIndexMap[inputDesc.SemanticName]++;
        }
		else
        {
		    semanticIndexMap[inputDesc.SemanticName] = 0;
        }

		// instanced
		string_view vertexAttributeName = inputDesc.SemanticName;
		bool bInstanced = std::find(context.InstancedAttributes.begin(), context.InstancedAttributes.end(), vertexAttributeName) != context.InstancedAttributes.end();
		if(bInstanced)
        {
		    inputDescriptions[i].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_INSTANCE_DATA;
		    inputDescriptions[i].InstanceDataStepRate = 1;
            inputDescriptions[i].AlignedByteOffset = 0;
        }
		else
        {
            inputDescriptions[i].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
            inputDescriptions[i].InstanceDataStepRate = 0;
            inputDescriptions[i].AlignedByteOffset = i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;
        }

		inputDescriptions[i].SemanticIndex = semanticIndexMap[inputDesc.SemanticName];
		inputDescriptions[i].InputSlot = 0;

		byteOffset += SSDXTranslator::GetDXGIFormatByteSize(format);
	}

	HR(dxDevice->CreateInputLayout(inputDescriptions, inputParamCount, mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), &mInputLayout));

	delete [] inputDescriptions;
}

bool SSDX11VertexShader::CompileFromFile(std::wstring filepath, const SSCompileContext& compileContext)
{
    ID3DBlob* errorMsg = nullptr;

    check(std::filesystem::exists(filepath));

    D3DCompileFromFile(filepath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VSMain", "vs_5_0", 0, 0, &mShaderBuffer, &errorMsg);

    if(errorMsg != nullptr)
    {
        PrintCompileError(errorMsg);

        ReleaseCOM(errorMsg);

        return false;
    }

    auto* dxDevice = SSDX11Renderer::Get().GetDevice();

    HR(dxDevice->CreateVertexShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mVertexShader));


    ID3D11ShaderReflection* vertexShaderReflection = nullptr;
    HR(D3DReflect(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**) &vertexShaderReflection));

    // @constant buffer reflection
    ReflectCompiledShader(vertexShaderReflection);

    // @input layout creation
    CreateInputLayout(vertexShaderReflection, compileContext);

    return true;
}

 bool SSDX11VertexShader::CompileFromFile(std::wstring filepath)
 {
    ID3DBlob* errorMsg = nullptr;

    check(std::filesystem::exists(filepath));

    D3DCompileFromFile(filepath.c_str(), nullptr, nullptr, "VSMain", "vs_5_0", 0, 0, &mShaderBuffer, &errorMsg);

    if(errorMsg != nullptr)
    {
		PrintCompileError(errorMsg);
    	
		ReleaseCOM(errorMsg);

        return false;
    }

    auto* dxDevice = SSDX11Renderer::Get().GetDevice();
    
	HR(dxDevice->CreateVertexShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mVertexShader));
	

    ID3D11ShaderReflection* vertexShaderReflection = nullptr;    
    HR(D3DReflect(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**) &vertexShaderReflection));  
	
	// @constant buffer reflection
	ReflectCompiledShader(vertexShaderReflection);

	SSCompileContext dummy;

	// @input layout creation
	CreateInputLayout(vertexShaderReflection, dummy);

    return true;
 }


void SSDX11VertexShader::SetTexture(ID3D11DeviceContext* deviceContext, std::string name, SSDX11Texture2D* texture)
{
	check(mTextureMap.count(name) > 0);
	check(deviceContext != nullptr);	

	UINT slotIndex = mTextureMap[name];	

	deviceContext->VSSetShaderResources(slotIndex, 1, texture->GetShaderResourceViewRef());
}

void SSDX11VertexShader::SetSampler(ID3D11DeviceContext* deviceContext, std::string name, ID3D11SamplerState* sampler)
{
	UINT slotIndex = mSamplerMap[name];

	check(deviceContext != nullptr);

	deviceContext->VSSetSamplers(slotIndex, 1, &sampler);
}

 void SSDX11VertexShader::SetSampler(std::string name, ID3D11SamplerState* sampler)
 { 	 
	 auto* dxDeviceContext = SSDX11Renderer::Get().GetImmediateDeviceContext();
	 
	 SetSampler(dxDeviceContext, name, sampler);
 }

bool SSDX11ComputeShader::CompileFromFile(std::wstring filepath)
{
	ID3D10Blob* errorMsg = nullptr;
	check(std::filesystem::exists(filepath));
	D3DCompileFromFile(filepath.c_str(), nullptr, nullptr, "CSMain", "cs_5_0", 0, 0, &mShaderBuffer, &errorMsg);
	if(errorMsg != nullptr)
	{
		PrintCompileError(errorMsg);
		return false;
	}
	auto* dxDevice = SSDX11Renderer::Get().GetDevice();
	HR(dxDevice->CreateComputeShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mComputeShader));
	// @constant buffer reflection
	ID3D11ShaderReflection* computeShaderReflection = nullptr;
	HR(D3DReflect(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&computeShaderReflection));
	ReflectCompiledShader(computeShaderReflection);
	return true;
}

SSDX11ComputeShader::~SSDX11ComputeShader()
{
	ReleaseCOM(mComputeShader);
}

#pragma region PixelShader

 SSDX11PixelShader::~SSDX11PixelShader() 
 {
	 for (auto& kvp : mConstantBufferMap)
	 {
		 if (kvp.second)
		 {
			// kvp.second->Destroy();
		 }
	 }
	 ReleaseCOM(mPixelShader);
 }

bool SSDX11PixelShader::CompileFromFile(std::wstring filepath, const SSCompileContext& context)
{
    ID3D10Blob* errorMsg = nullptr;

    check(std::filesystem::exists(filepath));

    std::vector<D3D_SHADER_MACRO> macros;

    for(auto& [name, define] : context.MacroDefines)
    {
        D3D_SHADER_MACRO macro{name.data(), define.data()};
        macros.push_back(macro);
    }

    // null terminate , otherwise crashes
    macros.push_back({nullptr,nullptr});

    D3DCompileFromFile(filepath.c_str(), macros.data(), nullptr, "PSMain", "ps_5_0", 0, 0, &mShaderBuffer, &errorMsg);

    if(errorMsg != nullptr)
    {
        PrintCompileError(errorMsg);
        return false;
    }

    auto* dxDevice = SSDX11Renderer::Get().GetDevice();

    HR(dxDevice->CreatePixelShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mPixelShader));

    // @constant buffer reflection
    ID3D11ShaderReflection* pixelShaderReflection = nullptr;
    HR(D3DReflect(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pixelShaderReflection));

    ReflectCompiledShader(pixelShaderReflection);

    return true;
}

bool SSDX11PixelShader::CompileFromFile(std::wstring filepath)
{    
    ID3D10Blob* errorMsg = nullptr;

    check(std::filesystem::exists(filepath));

    D3DCompileFromFile(filepath.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, &mShaderBuffer, &errorMsg);

    if(errorMsg != nullptr)
    {
        PrintCompileError(errorMsg);		
        return false;
    }

    auto* dxDevice = SSDX11Renderer::Get().GetDevice();
    HR(dxDevice->CreatePixelShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mPixelShader));

	// @constant buffer reflection
	ID3D11ShaderReflection* pixelShaderReflection = nullptr;
	HR(D3DReflect(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pixelShaderReflection));

	ReflectCompiledShader(pixelShaderReflection);

    return true;
}


void SSDX11PixelShader::SetTexture(ID3D11DeviceContext* deviceContext, std::string name, SSDX11Texture2D* texture)
{
	check(deviceContext != nullptr);

	if (mTextureMap.count(name) > 0 && texture != nullptr)
	{
		UINT slotIndex = mTextureMap[name];

		deviceContext->PSSetShaderResources(slotIndex, 1, texture->GetShaderResourceViewRef());
	}
}

void SSDX11PixelShader::SetTextureAsNull(ID3D11DeviceContext* deviceContext, std::string name)
{
	check(deviceContext != nullptr);

	if (mTextureMap.count(name) > 0)
	{
		UINT slotIndex = mTextureMap[name];		

		ID3D11ShaderResourceView* nullResourceView = nullptr;

		deviceContext->PSSetShaderResources(slotIndex, 1, &nullResourceView);
	}
}

void SSDX11PixelShader::SetTextureAsNull(std::string name)
{
	if (mTextureMap.count(name) > 0)
	{
		UINT slotIndex = mTextureMap[name];

		auto* dxDeviceContext = SSDX11Renderer::Get().GetImmediateDeviceContext();

		ID3D11ShaderResourceView* nullResourceView = nullptr;

		dxDeviceContext->PSSetShaderResources(slotIndex, 1, &nullResourceView);
	}
}


void SSDX11PixelShader::SetSampler(std::string name, ID3D11SamplerState* sampler)
{
	UINT slotIndex = mSamplerMap[name];

	auto* dxDeviceContext = SSDX11Renderer::Get().GetImmediateDeviceContext();

	dxDeviceContext->PSSetSamplers(slotIndex, 1, &sampler);
}

void SSDX11PixelShader::SetSampler(ID3D11DeviceContext* dxDeviceContext, std::string name, ID3D11SamplerState* sampler)
{
	UINT slotIndex = mSamplerMap[name];	

	dxDeviceContext->PSSetSamplers(slotIndex, 1, &sampler);
}

#pragma endregion