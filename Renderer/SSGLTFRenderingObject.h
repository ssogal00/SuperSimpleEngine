#pragma once

#include "SSRenderingObject.h"

class SSGLTFRenderingObject : public SSRenderingObject
{
public:
	SSGLTFRenderingObject(SSObjectBase* InGameObject);

private:
	class SSGLTFMeshObject* mGLTFMeshObject = nullptr;
};
