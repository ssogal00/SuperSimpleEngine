

#include "SSInstancingTestScene.h"
#include "SSSphere.h"
#include "SSGLTF.h"


void SSInstancingTestScene::InitializeScene()
{
	GLTF::SSGLTF_V2 Result = GLTF::SSGLTF_V2::LoadGLTFFile("./Resource/GLTF/FlightHelmet/glTF/FlightHelmet.gltf");


	SSInstancedVertexData instanceData;	

	const int x = 5;
	const int y = 5;
	int instanceCount = 0;
	for(int i = -x/2; i < x / 2; ++i)
	{
		for(int j = -y/2; j < y / 2; ++j)
		{
			float posX = 50.f;
			float posY = static_cast<float>(i * 20.0f);
			float posZ = static_cast<float>(j * 20.0f);
			InstancePositions.push_back(XMFLOAT3(posX, posY, posZ));
			instanceCount++;
		}
	}
	instanceData.InstanceCount = instanceCount;
	instanceData.InstanceStride = sizeof(XMFLOAT3);
	instanceData.InstanceDataPtr = InstancePositions.data();

	InstancedSphereObject = std::make_shared<SSInstancedSphere>(static_cast<UINT>(20), static_cast<UINT>(20), 2.0f,instanceData);
	InstancedSphereObject->SetPositionX(50);
}

void SSInstancingTestScene::DestroyScene()
{
	InstancedSphereObject.reset();
	InstancePositions.clear();
}

void SSInstancingTestScene::Tick(float DeltaSeconds)
{

}