#pragma once

#include "SSObjectBase.h"
#include "SSManagerBase.h"
#include <map>

class DX11RENDERER_API SSRenderingObjectManager : public Singleton<SSRenderingObjectManager>, public SSManagerBase
{
public:
	SSRenderingObjectManager();

	virtual void Initialize() override;
	virtual void Shutdown() override;


	void SetPendingObjects(std::map<UINT, SSObjectBase*> objectMap);

	void UpdateObjects();
	void Tick(float deltaTime);

	void DeleteAllObjects();
	
	const std::map<UINT, class SSRenderingObject*>& GetRenderingObjectMap() { return mRenderingObjectMap; }
	
private:
	std::map<UINT, class SSRenderingObject*> mRenderingObjectMap;
	std::map<UINT, class SSObjectBase*> mPendingObjectMap;

	CRITICAL_SECTION mCriticalSection;

};