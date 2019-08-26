#pragma once

#include "SSManagerBase.h"
#include "Singleton.h"

class SSRaterizeStateManager : public Singleton<SSRaterizeStateManager>, public SSManagerBase
{
public:
	virtual void Initialize() override;
	virtual void Shutdown() override;
	
	void SetToDefault();
	void SetCullModeNone();
	void SetFrontCounterClockwise();
	void SetWireFrameMode();
	
private:
	D3D11_RASTERIZER_DESC mDefaultDesc;
	D3D11_RASTERIZER_DESC mCullModeNoneDesc;
	D3D11_RASTERIZER_DESC mFrontCounterClockwiseDesc;
	D3D11_RASTERIZER_DESC mWireFrameDesc;

	ID3D11RasterizerState* mDefaultState = nullptr;
	ID3D11RasterizerState* mCullModeNoneState = nullptr;
	ID3D11RasterizerState* mFrontCounterClockwiseState = nullptr;
	ID3D11RasterizerState* mWireFrameState = nullptr;
};