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

private:
	std::string mGLTFFilePath;
	GLTF::SSGLTF_V2 mGLTFData;
};
