
#include "SSGLTFTestScene.h"



void SSGLTFTestScene::InitializeScene()
{
	TestMesh = std::make_shared<SSGLTFMeshObject>("./Resource/GLTF/DamagedHelmet/glTF/DamagedHelmet.gltf");

}

void SSGLTFTestScene::DestroyScene()
{

}

void SSGLTFTestScene::Tick(float DeltaSeconds)
{

}
