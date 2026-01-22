
#include "SSGLTFTestScene.h"



void SSGLTFTestScene::InitializeScene()
{
	TestMesh = std::make_shared<SSGLTFMeshObject>("./Resource/GLTF/FlightHelmet/glTF/FlightHelmet.gltf");

}

void SSGLTFTestScene::DestroyScene()
{

}

void SSGLTFTestScene::Tick(float DeltaSeconds)
{

}
