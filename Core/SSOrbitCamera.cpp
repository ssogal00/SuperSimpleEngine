#include "SSOrbitCamera.h"
#include <algorithm>
#include <math.h>

using namespace DirectX;

SSOrbitCamera::SSOrbitCamera()
	: SSCameraBase()
{
	mLookAtPoint = XMFLOAT3(0, 0, 0);
	mEyePosition = XMFLOAT3(0, 0, -mDistance);
	mUp = XMFLOAT3(0, 1, 0);

	mTargetYaw = mYaw;
	mTargetPitch = mPitch;
	mTargetDistance = mDistance;
}

void SSOrbitCamera::Update()
{
	UpdateRotationMatrix();
	UpdateViewMatrix();
	UpdateProjMatrix();
}

void SSOrbitCamera::Tick(float DeltaSeconds)
{
	const float t = (std::min)(DeltaSeconds * mRotationInterpSpeed, 1.0f);
	const float distanceT = (std::min)(DeltaSeconds * mDistanceInterpSpeed, 1.0f);

	mYaw += (mTargetYaw - mYaw) * t;
	mPitch += (mTargetPitch - mPitch) * t;
	mDistance += (mTargetDistance - mDistance) * distanceT;

	Update();
}

XMMATRIX SSOrbitCamera::GetTranslation() const
{
	return XMMatrixTranslation(mEyePosition.x, mEyePosition.y, mEyePosition.z);
}

void SSOrbitCamera::RotatePitch(float amount)
{
	mTargetPitch += amount;
}

void SSOrbitCamera::RotateYaw(float amount)
{
	mTargetYaw += amount;
}

void SSOrbitCamera::MoveFoward(float amount)
{
	SetDistance(mDistance - amount);
}

void SSOrbitCamera::MoveBackward(float amount)
{
	SetDistance(mDistance + amount);
}

void SSOrbitCamera::SetDistance(float InDistance)
{
	mTargetDistance = (InDistance < 0.1f) ? 0.1f : InDistance;
}

void SSOrbitCamera::UpdateRotationMatrix()
{
	mRotation = XMMatrixRotationZ(mPitch) * XMMatrixRotationY(mYaw);
}

void SSOrbitCamera::UpdateViewMatrix()
{
	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, mRotation);

	XMFLOAT3 forward;
	forward.x = matrix._11;
	forward.y = matrix._12;
	forward.z = matrix._13;

	mEyePosition.x = mLookAtPoint.x - forward.x * mDistance;
	mEyePosition.y = mLookAtPoint.y - forward.y * mDistance;
	mEyePosition.z = mLookAtPoint.z - forward.z * mDistance;

	mView = XMMatrixLookAtLH(XMLoadFloat3(&mEyePosition),
		XMLoadFloat3(&mLookAtPoint),
		XMLoadFloat3(&mUp));
}

void SSOrbitCamera::UpdateProjMatrix()
{
	mProj = XMMatrixPerspectiveFovLH(mFOV, mAspectRatio, mNear, mFar);
}