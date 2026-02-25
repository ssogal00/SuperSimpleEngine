#pragma once

#include <DirectXTex.h>
#include <wrl/internal.h>
#include <wrl/client.h>
#include <memory>
#include "SSTexture2DBase.h"

using Microsoft::WRL::ComPtr;

class SSDX11Texture2D : public SSTexture2DBase
{
public:	
	virtual ~SSDX11Texture2D();
	virtual ID3D11ShaderResourceView* GetShaderResourceView() { return mShaderResourceView.Get(); }
	virtual ID3D11ShaderResourceView* const* GetShaderResourceViewRef() { return mShaderResourceView.GetAddressOf(); }
	
	virtual ID3D11Texture2D* GetTextureResource() { return mTexturePtr.Get(); }
	virtual ID3D11Texture2D* const* GetTextureResourceRef() { return mTexturePtr.GetAddressOf(); }

	static std::shared_ptr<SSDX11Texture2D> CreateFromDDSFile( std::wstring filename, bool bSRGB = false);
	static std::shared_ptr<SSDX11Texture2D> CreateFromDDSFile( std::string filename, bool bSRGB = false);

	static std::shared_ptr<SSDX11Texture2D> CreateFromHDRFile( std::wstring filename, bool bSRGB = false);
	static std::shared_ptr<SSDX11Texture2D> CreateFromHDRFile( std::string filename, bool bSRGB = false);

	static std::shared_ptr<SSDX11Texture2D> CreateFromTGAFile( std::wstring filename, bool bSRGB = false);
	static std::shared_ptr<SSDX11Texture2D> CreateFromTGAFile( std::string filename, bool bSRGB = false);

	static std::shared_ptr<SSDX11Texture2D> CreateFromWICFile(std::wstring filename, bool bSRGB = false);
	static std::shared_ptr<SSDX11Texture2D> CreateFromWICFile(std::string filename, bool bSRGB = false);

	virtual bool LoadFromDDSFile( std::wstring filename, bool bSRGB = false) override;
	virtual bool LoadFromTGAFile( std::wstring filename, bool bSRGB = false) override;
	virtual bool LoadFromHDRFile( std::wstring filename, bool bSRGB = false) override;
	virtual bool LoadFromWICFile(std::wstring filename, bool bSRGB = false) override;

	DXGI_FORMAT GetTextureFormat() const { return mTextureFormat; }

protected:
	bool LoadInternal(const DirectX::TexMetadata& metaData, const DirectX::ScratchImage& image, bool bsrgb);

protected:
	DXGI_FORMAT mTextureFormat = DXGI_FORMAT_UNKNOWN;
	ComPtr<ID3D11Texture2D> mTexturePtr = nullptr;		
	ComPtr<ID3D11ShaderResourceView> mShaderResourceView = nullptr;	
	std::wstring DebugResourcePath;
};