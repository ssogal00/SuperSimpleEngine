#pragma once

#include "SSRenderCore.h"
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class RENDERCORE_API SSRenderer
{
public:
	SSRenderer();
	virtual void Initialize(HWND windowHandle) = 0;
	virtual void Shutdown() = 0;
	virtual void OnWindowResize(int newWidth, int newHeight) = 0;
	virtual void DrawScene() = 0;
	virtual void PauseRendering() { mPaused = true; }
	virtual void ResumeRendering() { mPaused = false; }
	virtual void UpdateRenderingObjects() = 0;
		

protected:
	virtual bool CreateDevice() = 0;
	virtual bool CreateSwapChain() = 0;

	int mBufferWidth = 1920;
	int mBufferHeight = 1080;
	HWND mWindowHandle;
	UINT m4xMSAAQuality = 0;
	bool mPaused = false;
};