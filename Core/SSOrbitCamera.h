#pragma once


#include "SSCameraBase.h"
#include "DirectXMath.h"

class CORE_API SSOrbitCamera : public SSCameraBase
{
public:
	SSOrbitCamera();
	virtual void Update() override;
	virtual void Tick(float DeltaSeconds);
	virtual DirectX::XMMATRIX GetTranslation() const override;
	virtual void RotatePitch(float amount) override;
	virtual void RotateYaw(float amount) override;
	virtual void MoveFoward(float amount) override;
	virtual void MoveBackward(float amount) override;

	virtual DirectX::XMFLOAT3 GetCameraLocation() const override { return mEyePosition; }

	void SetLookAtPoint(const DirectX::XMFLOAT3& InPoint) { mLookAtPoint = InPoint; }
	DirectX::XMFLOAT3 GetLookAtPoint() const { return mLookAtPoint; }

	void SetDistance(float InDistance);
	float GetDistance() const { return mDistance; }

	virtual void FocusOnPoint(const DirectX::XMFLOAT3& point) override;

protected:
	virtual void UpdateViewMatrix() override;
	virtual void UpdateProjMatrix() override;
	virtual void UpdateRotationMatrix() override;


	DirectX::XMFLOAT3 mLookAtPoint;
	DirectX::XMFLOAT3 mEyePosition;
	DirectX::XMFLOAT3 mUp;

	float mDistance = 10.0f;
	float mTargetDistance = 10.0f;

	float mTargetYaw = 0.0f;
	float mTargetPitch = 0.0f;
	float mRotationInterpSpeed = 8.0f;
	float mDistanceInterpSpeed = 8.0f;
};