#include "SSCore.h"
#include "SSFreeCamera.h"
#include "SSCameraManager.h"
#include "SSMathHelper.h"

SSCameraManager& SSCameraManager::Get()
{
	if(mInstance == nullptr)
	{
		mInstance = new SSCameraManager();
	}
	return *mInstance;
}

SSCameraManager* SSCameraManager::GetPtr()
{
	if (mInstance == nullptr)
	{
		mInstance = new SSCameraManager();
	}
	return mInstance;
}

SSCameraManager* SSCameraManager::mInstance = nullptr;


SSCameraManager::SSCameraManager()
{
    mCurrentCamera = new SSFreeCamera();
}

SSCameraManager::~SSCameraManager()
{
    if(mCurrentCamera)
    {
        delete mCurrentCamera;
    }
}

XMMATRIX SSCameraManager::GetCurrentCameraView() const
{
	check(mCurrentCamera != nullptr);
	return mCurrentCamera->GetView();
}

XMMATRIX SSCameraManager::GetCurrentCameraProj() const
{
	check(mCurrentCamera != nullptr);
	return mCurrentCamera->GetProj();
}

XMMATRIX SSCameraManager::GetCurrentCameraTranslation() const
{
	check(mCurrentCamera != nullptr);
	return mCurrentCamera->GetTranslation();
}

void SSCameraManager::RotateYaw(float amount)
{
	mCurrentCamera->RotateYaw(amount);
	UpdateCurrentCamera();
}

void SSCameraManager::RotatePitch(float amount)
{
	mCurrentCamera->RotatePitch(amount);
	UpdateCurrentCamera();
}

void SSCameraManager::MoveFoward(float amount)
{
	mCurrentCamera->MoveFoward(amount);
	UpdateCurrentCamera();
}

void SSCameraManager::MoveBackward(float amount)
{
	mCurrentCamera->MoveBackward(amount);
	UpdateCurrentCamera();
}

void SSCameraManager::UpdateMVP()
{
	XMMATRIX translation = GetCurrentCameraTranslation();
	XMMATRIX modelView = translation * GetCurrentCameraView();
	mCurrentMVP = modelView * GetCurrentCameraProj();
}

void SSCameraManager::Tick(float DeltaTime)
{
	UpdateCurrentCamera();

	mGameThreadMVP = mCurrentMVP;
	mGameThreadView = GetCurrentCameraView();
	mGameThreadProj = GetCurrentCameraProj();
}

XMMATRIX SSCameraManager::GetCurrentCameraMVP() const
{
	return mCurrentMVP;
}

void SSCameraManager::UpdateCurrentCamera()
{
	check(mCurrentCamera != nullptr);
	mCurrentCamera->Update();
	UpdateMVP();

}

void SSCameraManager::SetCurrentCameraFOV(float fov)
{
	check(mCurrentCamera != nullptr);
	mCurrentCamera->SetFOV(fov);
}

void SSCameraManager::IncreaseCurrentCameraFOV(float amount)
{
	float newFOV = mCurrentCamera->GetFOV() + amount;
	mCurrentCamera->SetFOV(newFOV);
}

void SSCameraManager::DecreaseCurrentCameraFOV(float amount)
{
	float newFOV = mCurrentCamera->GetFOV() - amount;

	if (newFOV <= 0)
	{
		newFOV = 0.001f;
	}

	mCurrentCamera->SetFOV(newFOV);
}



void SSCameraManager::SetCurrentCameraAspectRatio(float ratio)
{
	check(mCurrentCamera != nullptr);
	mCurrentCamera->SetAspectRatio(ratio);
}

XMFLOAT3 SSCameraManager::GetCurrentCameraPosition() const
{
	check(mCurrentCamera != nullptr);
	return mCurrentCamera->GetCameraLocation();
}