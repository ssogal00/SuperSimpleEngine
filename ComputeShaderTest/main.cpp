#include <d3d11.h>
#include <vector>
#include <iostream>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

struct float3 {
    float x, y, z;
};

// 오류 처리를 위한 매크로 (실제 사용 시 더 견고하게 작성 필요)
#define HR(x) if(FAILED(x)) { std::cout << "Error at line " << __LINE__ << std::endl; return -1; }

int main() {
    // --------------------------------------------------------
    // 1. DX11 디바이스 및 컨텍스트 초기화
    // --------------------------------------------------------
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;
    D3D_FEATURE_LEVEL featureLevel;

    HR(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
        D3D11_SDK_VERSION, &device, &featureLevel, &context));

    // --------------------------------------------------------
    // 2. Compute Shader 컴파일 및 생성
    // --------------------------------------------------------
    ID3D10Blob* csBlob = nullptr;
    ID3D10Blob* errorBlob = nullptr;

    // 셰이더 파일 이름은 "VectorSum.hlsl"로 가정
    HRESULT hr = D3DCompileFromFile(L"Shader/VectorSum.cs", nullptr, nullptr, "CSMain", "cs_5_0", 0, 0, &csBlob, &errorBlob);
    if (FAILED(hr)) {
        if (errorBlob) std::cout << (char*)errorBlob->GetBufferPointer() << std::endl;
        return -1;
    }

    ID3D11ComputeShader* computeShader = nullptr;
    HR(device->CreateComputeShader(csBlob->GetBufferPointer(), csBlob->GetBufferSize(), nullptr, &computeShader));
    csBlob->Release();

    // --------------------------------------------------------
    // 3. 데이터 준비 (Input Data)
    // --------------------------------------------------------
    const int dataCount = 1000;
    std::vector<float3> inputData(dataCount);
    for (int i = 0; i < dataCount; ++i) {
        inputData[i] = { 1.0f, 2.0f, 3.0f }; // 테스트용: 모두 (1, 2, 3)
    }

    // --------------------------------------------------------
    // 4. 버퍼 생성
    // --------------------------------------------------------

    // A. 입력 버퍼 (Structured Buffer, GPU Read)
    ID3D11Buffer* inputBuffer = nullptr;
    ID3D11ShaderResourceView* inputSRV = nullptr;

    D3D11_BUFFER_DESC inputDesc = {};
    inputDesc.ByteWidth = sizeof(float3) * dataCount;
    inputDesc.Usage = D3D11_USAGE_DEFAULT;
    inputDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    inputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    inputDesc.StructureByteStride = sizeof(float3);

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = inputData.data();

    HR(device->CreateBuffer(&inputDesc, &initData, &inputBuffer));

    // SRV 생성 (셰이더에서 읽기 위함)
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_UNKNOWN;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    srvDesc.Buffer.FirstElement = 0;
    srvDesc.Buffer.NumElements = dataCount;
    HR(device->CreateShaderResourceView(inputBuffer, &srvDesc, &inputSRV));

    // B. 출력 버퍼 (Structured Buffer, GPU Write)
    ID3D11Buffer* outputBuffer = nullptr;
    ID3D11UnorderedAccessView* outputUAV = nullptr;

    D3D11_BUFFER_DESC outputDesc = {};
    outputDesc.ByteWidth = sizeof(float3) * 1; // 결과는 1개
    outputDesc.Usage = D3D11_USAGE_DEFAULT;
    outputDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
    outputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    outputDesc.StructureByteStride = sizeof(float3);

    HR(device->CreateBuffer(&outputDesc, nullptr, &outputBuffer));

    // UAV 생성 (셰이더에서 쓰기 위함)
    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
    uavDesc.Format = DXGI_FORMAT_UNKNOWN;
    uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    uavDesc.Buffer.FirstElement = 0;
    uavDesc.Buffer.NumElements = 1;
    HR(device->CreateUnorderedAccessView(outputBuffer, &uavDesc, &outputUAV));

    // C. CPU 읽기용 버퍼 (Staging Buffer)
    ID3D11Buffer* stagingBuffer = nullptr;
    D3D11_BUFFER_DESC stagingDesc = {};
    stagingDesc.ByteWidth = sizeof(float3) * 1;
    stagingDesc.Usage = D3D11_USAGE_STAGING;       // CPU 접근 가능
    stagingDesc.BindFlags = 0;
    stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

    HR(device->CreateBuffer(&stagingDesc, nullptr, &stagingBuffer));

    // --------------------------------------------------------
    // 5. Compute Shader 실행
    // --------------------------------------------------------
    context->CSSetShader(computeShader, nullptr, 0);
    context->CSSetShaderResources(0, 1, &inputSRV);
    context->CSSetUnorderedAccessViews(0, 1, &outputUAV, nullptr);

    // 스레드 그룹 실행 (1000개를 처리하기 위해 1개의 그룹만 필요)
    // 셰이더 내부에서 [numthreads(1024, 1, 1)]로 정의했으므로 1x1x1 디스패치
    context->Dispatch(1, 1, 1);

    // --------------------------------------------------------
    // 6. 결과 회수 (GPU -> CPU)
    // --------------------------------------------------------

    // Output Buffer의 내용을 Staging Buffer로 복사
    context->CopyResource(stagingBuffer, outputBuffer);

    // Staging Buffer 맵핑하여 데이터 읽기
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HR(context->Map(stagingBuffer, 0, D3D11_MAP_READ, 0, &mappedResource));

    float3* result = reinterpret_cast<float3*>(mappedResource.pData);
    std::cout << "Result: " << result->x << ", " << result->y << ", " << result->z << std::endl;
    // 예상 결과: 1000.0, 2000.0, 3000.0

    context->Unmap(stagingBuffer, 0);

    // --------------------------------------------------------
    // 7. 리소스 해제
    // --------------------------------------------------------
    if (stagingBuffer) stagingBuffer->Release();
    if (outputUAV) outputUAV->Release();
    if (outputBuffer) outputBuffer->Release();
    if (inputSRV) inputSRV->Release();
    if (inputBuffer) inputBuffer->Release();
    if (computeShader) computeShader->Release();
    if (context) context->Release();
    if (device) device->Release();

    return 0;
}