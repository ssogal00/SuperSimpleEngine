#pragma once


class SSRHIPixelShader
{
public:
	SSRHIPixelShader();

private:
	ID3D11Shader* mpPixelShader = nullptr;
};