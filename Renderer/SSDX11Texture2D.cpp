
#include "SSRendererModulePCH.h"
#include "SSDX11Texture2D.h"
#include "DirectXTex.h"
#include "SSDX11Renderer.h"
#include "SSDX11RenderTarget.h"

SSDX11Texture2D::~SSDX11Texture2D()
{
	if (mTexturePtr)
	{
		mTexturePtr.Reset();
	}
	if (mShaderResourceView)
	{
		mShaderResourceView.Reset();
	}
}

bool SSDX11Texture2D::LoadFromHDRFile(std::wstring filename, bool bsrgb)
{
	DirectX::TexMetadata metaData;
	DirectX::ScratchImage image;

	HRESULT result = DirectX::LoadFromHDRFile(filename.c_str(), &metaData, image);

	if (result != S_OK)
	{
		return false;
	}

	check(metaData.dimension == DirectX::TEX_DIMENSION_TEXTURE2D);

	return LoadInternal(metaData, image, bsrgb);
}

bool SSDX11Texture2D::LoadFromTGAFile(std::wstring filename, bool bsrgb)
{
	DirectX::TexMetadata metaData;
	DirectX::ScratchImage image;

	HRESULT result = DirectX::LoadFromTGAFile(filename.c_str(), &metaData, image);

	if (result != S_OK)
	{
		return false;
	}

	check(metaData.dimension == DirectX::TEX_DIMENSION_TEXTURE2D);

	return LoadInternal(metaData, image, bsrgb);
}

bool SSDX11Texture2D::LoadFromDDSFile(std::wstring filename, bool bsrgb)
{
	DirectX::TexMetadata metaData;
	DirectX::ScratchImage image;

	HRESULT result = (DirectX::LoadFromDDSFile(filename.c_str(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, &metaData, image));
	if (result != S_OK)
	{
		return false;
	}

	check(metaData.dimension == DirectX::TEX_DIMENSION_TEXTURE2D);

	return LoadInternal(metaData, image, bsrgb);
}


bool SSDX11Texture2D::LoadFromWICFile(std::wstring filename, bool bsrgb)
{
	
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		return false;
	}

	DirectX::TexMetadata metaData;
	DirectX::ScratchImage image;
	HRESULT result = DirectX::LoadFromWICFile(filename.c_str(), DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &metaData, image);
	if (result != S_OK)
	{
		return false;
	}
	check(metaData.dimension == DirectX::TEX_DIMENSION_TEXTURE2D);
	
	CoUninitialize();

	return LoadInternal(metaData, image, bsrgb);
}



bool SSDX11Texture2D::LoadInternal(const DirectX::TexMetadata& metaData, const DirectX::ScratchImage& image, bool bsrgb)
{
	mWidth = static_cast<UINT>(metaData.width);
	mHeight = static_cast<UINT>(metaData.height);
	mMipLevels = static_cast<UINT>(metaData.mipLevels);

	D3D11_TEXTURE2D_DESC description;
	description.Width = mWidth = static_cast<UINT>(metaData.width);
	description.Height = mHeight = static_cast<UINT>(metaData.height);
	description.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	description.MiscFlags = 0;
	description.Usage = D3D11_USAGE_DEFAULT;
	description.SampleDesc.Count = 1;
	description.SampleDesc.Quality = 0;
	description.MipLevels = static_cast<UINT>(metaData.mipLevels);
	description.ArraySize = static_cast<UINT>(metaData.arraySize);
	description.CPUAccessFlags = 0;

	this->bSRGB = bsrgb;

	if (bSRGB)
	{
		mTextureFormat = description.Format = DirectX::MakeSRGB(metaData.format);
	}
	else
	{
		mTextureFormat = description.Format = metaData.format;
	}

	//
	HR(SSDX11Renderer::Get().GetDevice()->CreateTexture2D(&description, nullptr, &mTexturePtr));

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
	ZeroMemory(&resourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	resourceViewDesc.Format = description.Format;
	resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceViewDesc.Texture2D.MostDetailedMip = 0;
	resourceViewDesc.Texture2D.MipLevels = static_cast<UINT>(metaData.mipLevels);

	HR(SSDX11Renderer::Get().GetDevice()->CreateShaderResourceView(mTexturePtr.Get(), &resourceViewDesc, &mShaderResourceView));

	// update lod data
	for (int i = 0; i < metaData.mipLevels; ++i)
	{
		auto* pLodImage = image.GetImage(i, 0, 0);
		
		if (pLodImage)
		{
			auto dstSubresource = D3D11CalcSubresource(i, 0, static_cast<UINT>(metaData.mipLevels));

			SSDX11Renderer::Get().GetImmediateDeviceContext()->UpdateSubresource(mTexturePtr.Get(), dstSubresource, nullptr, pLodImage->pixels, static_cast<UINT>(pLodImage->rowPitch), 0);
		}
	}

	return true;
}

std::shared_ptr<SSDX11Texture2D> SSDX11Texture2D::CreateFromDDSFile(std::wstring filename, bool bsrgb)
{
	std::shared_ptr<SSDX11Texture2D> texture = std::make_shared<SSDX11Texture2D>();

	if (texture->LoadFromDDSFile(filename, bsrgb))
	{
		return texture;
	}
	else
	{
		return nullptr;
	}
}

std::shared_ptr<SSDX11Texture2D> SSDX11Texture2D::CreateFromDDSFile(std::string filename, bool bsrgb)
{
	std::wstring wfilename;
	wfilename.assign(filename.begin(), filename.end());

	return SSDX11Texture2D::CreateFromDDSFile(wfilename, bsrgb);
}


std::shared_ptr<SSDX11Texture2D> SSDX11Texture2D::CreateFromTGAFile(std::wstring filename, bool bsrgb)
{
	std::shared_ptr<SSDX11Texture2D> texture = std::make_shared<SSDX11Texture2D>();

	if (texture->LoadFromTGAFile(filename, bsrgb))
	{
		return texture;
	}
	else
	{
		return nullptr;
	}
}


std::shared_ptr<SSDX11Texture2D> SSDX11Texture2D::CreateFromHDRFile(std::wstring filename, bool bsrgb)
{
	std::shared_ptr<SSDX11Texture2D> texture = std::make_shared<SSDX11Texture2D>();

	if (texture->LoadFromHDRFile(filename, bsrgb))
	{
		return texture;
	}
	else
	{
		check(false);
		return nullptr;
	}
}

std::shared_ptr<SSDX11Texture2D> SSDX11Texture2D::CreateFromHDRFile(std::string filename, bool bsrgb)
{
	std::wstring wfilename;
	wfilename.assign(filename.begin(), filename.end());

	return SSDX11Texture2D::CreateFromHDRFile(wfilename, bsrgb);
}


std::shared_ptr<SSDX11Texture2D> SSDX11Texture2D::CreateFromTGAFile(std::string filename, bool bsrgb)
{
	std::wstring wfilename;
	wfilename.assign(filename.begin(), filename.end());

	return SSDX11Texture2D::CreateFromTGAFile(wfilename, bsrgb);
}

std::shared_ptr<SSDX11Texture2D> SSDX11Texture2D::CreateFromWICFile(std::string filename, bool bsrgb)
{
	std::wstring wfilename;
	wfilename.assign(filename.begin(), filename.end());

	return SSDX11Texture2D::CreateFromWICFile(wfilename, bsrgb);
}

std::shared_ptr<SSDX11Texture2D> SSDX11Texture2D::CreateFromWICFile(std::wstring filename, bool bsrgb)
{
	std::shared_ptr<SSDX11Texture2D> texture = std::make_shared<SSDX11Texture2D>();
	if (texture->LoadFromWICFile(filename, bsrgb))
	{
		return texture;
	}
	else
	{
		return nullptr;
	}
}