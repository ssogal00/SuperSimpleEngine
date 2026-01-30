
#pragma once

#include "SSCameraBase.h"

using namespace DirectX;

class CORE_API SSCameraManager
{
public:
	SSCameraManager();
    ~SSCameraManager();

	void Tick(float deltaTime);

	static SSCameraManager& Get();
	static SSCameraManager* GetPtr();

	XMMATRIX GetCurrentCameraView() const;
	XMMATRIX GetCurrentCameraProj() const;
	XMMATRIX GetCurrentCameraViewProj() const;
	XMMATRIX GetCurrentCameraMVP() const;
	XMMATRIX GetCurrentCameraTranslation() const;

	// use this when u want to get mvp matrix in rendering thread
	// last model view projection matrix in game thread
	XMMATRIX GetGameThreadCameraMVP() const { return mGameThreadMVP; }
	XMMATRIX GetGameThreadCameraView() const { return mGameThreadView; }	
	XMMATRIX GetGameThreadCameraProj() const { return mGameThreadProj; }

	void IncreaseCurrentCameraFOV(float amount);
	void DecreaseCurrentCameraFOV(float amount);
	void SetCurrentCameraFOV(float fov);
	void SetCurrentCameraAspectRatio(float ratio);

	void MoveUp(float fAmount);
	void MoveDown(float fAmount);
	void MoveFoward(float amount);
	void MoveBackward(float amount);
	void RotateYaw(float amount);
	void RotatePitch(float amount);

    class SSCameraBase* GetCurrentCamera() { return mCurrentCamera; }

	XMFLOAT3 GetCurrentCameraPosition() const;

protected:
	void UpdateCurrentCamera();
	virtual void UpdateMVP();

    class SSCameraBase* mCurrentCamera = nullptr;
	static SSCameraManager* mInstance;
	XMMATRIX mCurrentMVP;

	XMMATRIX mGameThreadMVP;
	XMMATRIX mGameThreadView;
	XMMATRIX mGameThreadProj;
};