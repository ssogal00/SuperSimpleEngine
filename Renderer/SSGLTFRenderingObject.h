#pragma once

#include "SSRendererModulePCH.h"
#include "SSGLTFMesh.h"
#include "SSRenderingObject.h"

class SSGLTFRenderingObject : public SSRenderingObject
{
public:
	SSGLTFRenderingObject(SSObjectBase* InGameObject);

private:
	class SSGLTFMeshObject* mGLTFMeshObject = nullptr;
};
