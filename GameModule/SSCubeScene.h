#pragma once

#include "SSGameScene.h"


class GAMEMODULE_API SSCubeScene : public SSGameScene
{
public:
	void InitializeScene() override;
	void DestroyScene() override;
	virtual void Tick(float DeltaSeconds) override;
protected:
	
	class std::shared_ptr<class SSSphere> mSphere = nullptr;
	class std::shared_ptr<class SSPBRSphere> mPBRSphere = nullptr;

	class std::vector<std::shared_ptr<class SSSphere>> mPBRSphereArray;
	
	float Elapsed = 0;
};

