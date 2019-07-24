#include "Core.h"
#include "SSSceneObject.h"


XMMATRIX SSSceneObject::GetModelTransform()
{
	return XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z) * XMMatrixScaling(mScale.x, mScale.y, mScale.z);
}

void SSSceneObject::SetPositionX(float x)
{
	mPosition.x = x;
}

void SSSceneObject::SetPositionY(float y)
{
	mPosition.y = y;
}

void SSSceneObject::SetPositionZ(float z)
{
	mPosition.z = z;
}

void SSSceneObject::SetPosition(float x, float y, float z)
{
	mPosition.x = x; mPosition.y = y; mPosition.z = z;
}
void SSSceneObject::SetScale(float x, float y, float z)
{
	mScale.x = x; mScale.y = y; mScale.z = z;
}
