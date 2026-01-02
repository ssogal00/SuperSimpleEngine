

#include "SSInstancingTestScene.h"
#include "SSSphere.h"

void SSInstancingTestScene::InitializeScene()
{
	SSInstancedVertexData instanceData;	

	const int instanceCount = 100;

	for(int i = 0; i < instanceCount; ++i)
	{
		float x = static_cast<float>((i % 10) * 2.0f);
		float y = 0.0f;
		float z = static_cast<float>((i / 10) * 2.0f);
		InstancePositions.push_back(XMFLOAT3(x, y, z));
	}

	instanceData.InstanceCount = instanceCount;
	instanceData.InstanceStride = sizeof(XMFLOAT3);
	instanceData.InstanceDataPtr = InstancePositions.data();

	InstancedSphereObject = std::make_shared<SSInstancedSphere>(static_cast<UINT>(20), static_cast<UINT>(20), 2.0f,instanceData);
}

void SSInstancingTestScene::DestroyScene()
{
	InstancedSphereObject.reset();
	InstancePositions.clear();
}

void SSInstancingTestScene::Tick(float DeltaSeconds)
{

}