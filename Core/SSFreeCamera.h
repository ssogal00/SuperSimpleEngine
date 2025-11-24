#pragma once


#include "SSCameraBase.h"
#include "DirectXMath.h"

class CORE_API SSFreeCamera : public SSCameraBase
{
public:
	SSFreeCamera();
	virtual void Update() override;
	virtual DirectX::XMMATRIX GetTranslation() const override;
	virtual void RotatePitch(float amount) override;
	virtual void RotateYaw(float amount) override;
	virtual void MoveFoward(float amount) override;
	virtual void MoveBackward(float amount) override;

	virtual DirectX::XMFLOAT3 GetCameraLocation() const { return mEyePosition; }

protected:
	virtual void UpdateViewMatrix() override;
	virtual void UpdateProjMatrix() override;
	virtual void UpdateRotationMatrix() override;
	
	
	DirectX::XMFLOAT3 mLookAtPosition;
	DirectX::XMFLOAT3 mEyePosition;
	DirectX::XMFLOAT3 mUp;	
};

template<class T>
class TestTemplateClass
{
public:
	static T& Get()
	{
		if (mInstance == nullptr)
		{
			mInstance = new T();
		}
		return *mInstance;
	}
private:
	static T* mInstance;
};

template<class T>
T* TestTemplateClass<T>::mInstance = nullptr;
