#pragma once


class SSRHIPixelShader
{
public:
	SSRHIPixelShader();

private:
	ID3D11Shader* mpPixelShader = nullptr;
};

class SSRHIVertexShader
{
public:
	SSRHIVertexShader();

private:
	ID3D11Shader* mpVertexShader = nullptr;
};