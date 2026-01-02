
#include "SSCore.h"
#include "SSObjectBase.h"

using namespace DirectX;

SSObjectBase::SSObjectBase()
{
	mPosition = XMFLOAT3(0, 0, 0);
	mScale = XMFLOAT3(1, 1, 1);
	mMaterialProxy = std::make_shared<SSMaterialProxy>();
}

SSObjectBase::~SSObjectBase()
{
}

const SSMeshVertexIndexData& SSObjectBase::GetVertexData()
{
	return mVertexData;
}


std::shared_ptr<SSMaterialProxy> SSObjectBase::GetMaterialProxySharedPtr()
{
	return mMaterialProxy->shared_from_this();
}


XMMATRIX SSObjectBase::GetModelTransform()
{
	return XMMatrixScaling(mScale.x, mScale.y, mScale.z) 
		* XMMatrixRotationY(mYaw) 
		* XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
}

void SSObjectBase::SetPositionX(float x)
{
	mPosition.x = x;
}

void SSObjectBase::SetPositionY(float y)
{
	mPosition.y = y;
}

void SSObjectBase::SetPositionZ(float z)
{
	mPosition.z = z;
}

void SSObjectBase::SetScaleX(float x)
{
	mScale.x = x;
}

void SSObjectBase::SetScaleY(float y)
{
	mScale.y = y;
}

void SSObjectBase::SetScaleZ(float z)
{
	mScale.z = z;
}

void SSObjectBase::SetPosition(float x, float y, float z)
{
	mPosition.x = x; mPosition.y = y; mPosition.z = z;
}
void SSObjectBase::SetScale(float x, float y, float z)
{
	mScale.x = x; mScale.y = y; mScale.z = z;
}
