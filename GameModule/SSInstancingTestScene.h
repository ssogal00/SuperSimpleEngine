#pragma once

#include "SSGameScene.h"
#include <memory>

using namespace DirectX;


class GAMEMODULE_API SSInstancingTestScene : public SSGameScene
{
public:
	void InitializeScene() override;
	void DestroyScene() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	class std::shared_ptr<class SSInstancedSphere> InstancedSphereObject;
	std::vector<XMFLOAT3> InstancePositions;
	float Elapsed = 0;
};
