#include "SSGLTFTestScene.h"

void SSGLTFTestScene::InitializeScene()
{
	//TestMesh = std::make_shared<SSGLTFMeshObject>("./Resource/GLTF/FlightHelmet/glTF/FlightHelmet.gltf");

	TestMesh = std::make_shared<SSGLTFTestObject>();
	TestMesh->SetYaw(-DirectX::XM_PI / 2);
	TestMesh->SetPositionY(-3);
}

void SSGLTFTestScene::DestroyScene()
{

}

void SSGLTFTestScene::Tick(float DeltaSeconds)
{

}
