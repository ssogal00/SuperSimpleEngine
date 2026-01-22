#pragma once


#include "SSGameScene.h"
#include <memory>
#include "SSGLTF.h"
#include "SSGLTFMesh.h"

using namespace DirectX;

class GAMEMODULE_API SSGLTFTestScene : public SSGameScene
{
public:
	void InitializeScene() override;
	void DestroyScene() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	std::shared_ptr<SSGLTFMeshObject> TestMesh;
	float Elapsed = 0;
};
