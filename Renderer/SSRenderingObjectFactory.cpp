

#include "SSRenderingObjectFactory.h"
#include "SSGameObject.h"
#include "SSRenderingObject.h"
#include "SSGLTFRenderingObject.h"

SSRenderingObject* SSRenderingObjectFactory::CreateRenderingObject(SSObjectBase* InGameObject)
{
	if(InGameObject->GetObjectType() == "GLTFMeshObject")
	{
		return new SSGLTFRenderingObject(InGameObject);
	}

	return new SSRenderingObject(InGameObject);
}