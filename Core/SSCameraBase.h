#pragma once

#include "SSCore.h"
#include <DirectXMath.h>

class CORE_API SSCameraBase
{
public:
	SSCameraBase()
		: mFOV(DirectX::XM_PI * 0.25f)
		, mAspectRatio(1.0f)
		, mNear (1.f)
		, mFar (2500.0f)
	{
	}

	SSCameraBase(float fov, float aspectRatio, float fNear, float fFar)
		: mFOV(fov), mAspectRatio(aspectRatio), mNear(fNear), mFar(fFar)
	{		
	}

	virtual DirectX::XMMATRIX GetView() const { return mView; }
	virtual DirectX::XMMATRIX GetProj() const { return mProj; }
	virtual DirectX::XMMATRIX GetViewProj() const;
	virtual DirectX::XMMATRIX GetTranslation() const;
	virtual DirectX::XMMATRIX GetRotation() const;
	virtual DirectX::XMFLOAT3 GetCameraLocation() const = 0;
	
	virtual void RotateYaw(float amount){}
	virtual void RotatePitch(float amount){}
	virtual void MoveFoward(float amount){}
	virtual void MoveBackward(float amount){}
	virtual void MoveUp(float amount) {}
	virtual void MoveDown(float amount) {}

	void SetFOV(float fov) { mFOV = fov; }
	float GetFOV() { return mFOV; }
	void SetAspectRatio(float ratio) { mAspectRatio = ratio; }

	virtual ~SSCameraBase() {}

	virtual void Update(){}

protected:

	virtual void UpdateViewMatrix(){}
	virtual void UpdateProjMatrix(){}
	virtual void UpdateRotationMatrix(){}
	
	float mFOV;
	float mAspectRatio;
	float mNear;
	float mFar;
	float mYaw = 0;
	float mPitch = 0;

	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProj;
	DirectX::XMMATRIX mRotation;
};
