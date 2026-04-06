#pragma once

#include <memory>
#include "Singleton.h"
#include "SSManagerBase.h"
#include "SSName.h"
#include <map>

class SSDX11StreamingTexture2D;

class SSTextureManager : public Singleton<SSTextureManager>, public SSManagerBase
{
public:

	std::shared_ptr<class SSDX11Texture2D> LoadTexture2D(ID3D11DeviceContext* deviceContext, SSName texturepath, bool bSRGB = false);

	// Streaming texture: loads lowest mip first, progressively uploads higher mips
	// Only supports DDS files. Other formats fall back to synchronous LoadTexture2D.
	std::shared_ptr<SSDX11StreamingTexture2D> LoadStreamingTexture2D(SSName texturepath, bool bSRGB = false);

private:
	std::map<SSName, std::shared_ptr<class SSDX11Texture2D>> m2DTextureMap;
	std::map<SSName, std::shared_ptr<class SSTextureCube>> mCubeTextureMap;
	std::map<SSName, std::shared_ptr<SSDX11StreamingTexture2D>> mStreamingTextureMap;
};
