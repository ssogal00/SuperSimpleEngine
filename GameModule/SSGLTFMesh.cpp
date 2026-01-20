

#include "SSGLTFMesh.h"

SSGLTFMeshObject::SSGLTFMeshObject(std::string InGLTFFilePath)
	: mGLTFFilePath(InGLTFFilePath)
{
	 mGLTFData = GLTF::SSGLTF_V2::LoadGLTFFile(InGLTFFilePath);
}

SSGLTFMeshObject::~SSGLTFMeshObject()
{

}
