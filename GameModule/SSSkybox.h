#pragma once

#include "SSGameObject.h"

class SSSkybox :public SSGameObject
{
public:
	SSSkybox();
	virtual ~SSSkybox();
	virtual void CreateRenderData() override;
	virtual void CreateVertexData() override;
};