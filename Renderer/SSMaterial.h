#pragma once

#include "SSMaterialProxy.h"

class DX11RENDERER_API SSMaterial
{
public:
	SSMaterial(const SSMaterialProxy& InProxy);
	~SSMaterial();
	void ApplyProxyData(const SSMaterialProxy& InProxy);

private:

};