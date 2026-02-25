#include "SSRendererModulePCH.h"
#include "SSTextureManager.h"
#include "SSDX11Texture2D.h"



std::shared_ptr<class SSDX11Texture2D> SSTextureManager::LoadTexture2D(ID3D11DeviceContext* deviceContext, SSName texturepath, bool bSRGB)
{
	if (m2DTextureMap.count(texturepath) > 0)
	{
		return m2DTextureMap[texturepath];
	}
	else
	{
		std::string path = texturepath.ToString();
		std::shared_ptr<SSDX11Texture2D> texture;
		
		if (path.find(".dds") != std::string::npos)
		{
			texture = SSDX11Texture2D::CreateFromDDSFile( path, bSRGB);

			if (texture == nullptr)
			{
				check(false);
				return nullptr;
			}

			m2DTextureMap[path] = texture;

			return texture;
		}
		else if (path.find(".tga") != std::string::npos)
		{
			texture = SSDX11Texture2D::CreateFromTGAFile( path, bSRGB);

			if (texture == nullptr)
			{
				check(false);
				return nullptr;
			}

			m2DTextureMap[path] = texture;

			return texture;
		}
		else if (path.find(".hdr") != std::string::npos)
		{
			texture = SSDX11Texture2D::CreateFromHDRFile( path, bSRGB);

			if (texture == nullptr)
			{
				check(false);
				return nullptr;
			}

			m2DTextureMap[path] = texture;

			return texture;
		}
		else if (path.find(".png") != std::string::npos || path.find(".jpg") != std::string::npos || path.find(".jpeg") != std::string::npos)
		{
			texture = SSDX11Texture2D::CreateFromWICFile(path, bSRGB);
			if (texture == nullptr)
			{
				check(false);
				return nullptr;
			}
			m2DTextureMap[path] = texture;
			return texture;
		}
	}
	check(false);
	return nullptr;
}