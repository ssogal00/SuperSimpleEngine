#pragma once

#include <string>
#include "SSRenderThreadResidentObject.h"

class RENDERCORE_API SSTexture2DBase : public SSRenderThreadResidentObject
{
public:
	unsigned int GetWidth() const { return mWidth; }
	unsigned int GetHeight() const { return mHeight; }	
	unsigned int GetMipLevels() const { return mMipLevels; }
	
	virtual bool LoadFromDDSFile(std::wstring filename, bool bSRGB = false)=0;
	virtual bool LoadFromTGAFile(std::wstring filename, bool bSRGB = false)=0;
	virtual bool LoadFromHDRFile(std::wstring filename, bool bSRGB = false)=0;

protected:
	unsigned int mWidth;
	unsigned int mHeight;
	unsigned int mMipLevels = 1;
	bool bSRGB = false;
};