#pragma once

#include <windows.h>
#include "SSGameObject.h"
#include "SSGLTF.h"

class GAMEMODULE_API SSGLTFMeshObject : public SSGameObject
{
public:
	
	SSGLTFMeshObject(std::string InGLTFFilePath);

	virtual ~SSGLTFMeshObject();

	virtual std::string GetObjectType() const override { return "GLTFMeshObject"; }
	
	GLTF::SSGLTF_V2 mGLTFData;

protected:
	SSGLTFMeshObject() = default;

private:

	std::string mGLTFFilePath;
	
};


class GAMEMODULE_API SSGLTFTestObject : public SSGLTFMeshObject
{
public:
	SSGLTFTestObject();
};