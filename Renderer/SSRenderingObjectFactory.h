
#pragma once

class SSRenderingObject;
class SSGameObject;

class SSRenderingObjectFactory
{
public:
	static SSRenderingObject* CreateRenderingObject(class SSObjectBase* InGameObject);
};
