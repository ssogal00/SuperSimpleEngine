
#include "SSCube.h"
#include "SSSphere.h"
#include "SSCubeScene.h"
#include "SSGLTF.h"

void SSCubeScene::InitializeScene()
{
	for (int i = 0; i < 5; ++i)
	{
		std::shared_ptr<SSSphere> newSphere = std::make_shared<SSSphere>(static_cast<UINT>(20), static_cast<UINT>(20), 10.0f);
		newSphere->SetPositionX(40);
		//newSphere->SetPositionZ(i * 30);
		newSphere->SetPositionY( i* 20);
		mPBRSphereArray.push_back(newSphere);
	}

	//mPBRSphere = std::make_shared<SSPBRSphere>();
	//mPBRSphere->SetPositionX(40);
	//mPBRSphere->SetPositionY(-30);

	GLTF::SSGLTF_V2::LoadGLTFFile("./Resource/GLTF/DamagedHelmet/glTF/DamagedHelmet.gltf");
}

void SSCubeScene::DestroyScene()
{
	mSphere.reset();
	//mPBRSphere.reset();
}


void SSCubeScene::Tick(float DeltaSeconds)
{
	Elapsed += DeltaSeconds;

	if (Elapsed > 3)
	{
		Elapsed = 0;
	}
}