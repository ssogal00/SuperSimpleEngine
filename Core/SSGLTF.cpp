#include "SSGLTF.h"
#include "simdjson.h"
#include "simdjson/error.h"

namespace GLTF {
	SSGLTF_V2 SSGLTF_V2::LoadGLTFFile(const std::string& InPath)
	{
		simdjson::dom::parser Parser;
		simdjson::dom::element GLTFJson = Parser.load(InPath);

		SSGLTF_V2 Result{};

		for(auto Element: GLTFJson["meshes"].get_array())
		{
			auto JsonObject = Element.get_object();
			Mesh MeshObject{};
			MeshObject.Name = JsonObject["name"].get_string().take_value();
			for (auto PrimitiveElement : JsonObject["primitives"].get_array())
			{
				auto PrimitiveObject = PrimitiveElement.get_object();
				Primitive Prim{};
				// Prim.Mode = static_cast<int>(PrimitiveObject["mode"].get_int64().take_value());
				auto AttributesObject = PrimitiveObject["attributes"].get_object();
				for (auto Attribute : AttributesObject)
				{
					auto Key = Attribute.key;
					auto Value = static_cast<int>(Attribute.value.get_int64().take_value());
					Prim.Attributes[std::string(Key)] = Value;
				}
				Prim.Indices = static_cast<int>(PrimitiveObject["indices"].get_int64().take_value());
				Prim.Material = static_cast<int>(PrimitiveObject["material"].get_int64().take_value());
				MeshObject.Primitives.push_back(Prim);
			}
			Result.Meshes.push_back(MeshObject);
		}

		for (auto Element : GLTFJson["accessors"].get_array())
		{
			auto JsonObject = Element.get_object();
			auto TypeString = JsonObject["type"].get_string();

			Accessor AccessorObject{};
			AccessorObject.BufferView = static_cast<int>(JsonObject["bufferView"].get_int64().take_value());
			AccessorObject.ComponentType = static_cast<ComponentType>(JsonObject["componentType"].get_int64().take_value());
			AccessorObject.Count = static_cast<int>(JsonObject["count"].get_int64().take_value());
			
			if (JsonObject["name"].get_string().error() == simdjson::error_code::SUCCESS)
			{
				AccessorObject.Name = JsonObject["name"].get_string().value();
			}
			Result.Accessors.push_back(AccessorObject);
		}

		for (auto Element : GLTFJson["bufferViews"].get_array())
		{
			auto JsonObject = Element.get_object();

			BufferView View{};
			View.Buffer = JsonObject["buffer"].get_int64().take_value();
			View.ByteLength = JsonObject["byteLength"].get_int64().take_value();
			View.ByteOffset =  JsonObject["byteOffset"].get_int64().take_value();
			View.Target = JsonObject["target"].get_int64().take_value();
			if (JsonObject["name"].get_string().error() == simdjson::error_code::SUCCESS)
			{
				View.Name = JsonObject["name"].get_string().take_value();
			}

			Result.BufferViews.push_back(View);
		}

		for (auto Element : GLTFJson["buffers"].get_array())
		{
			auto JsonObject = Element.get_object();
			auto ByteLength = JsonObject["byteLength"];
			auto Uri = JsonObject["uri"];

			Buffer BufferObject{};
			BufferObject.ByteLength = static_cast<int>(ByteLength.get_int64().take_value());
			BufferObject.Uri = Uri.get_string().take_value();
			Result.Buffers.push_back(BufferObject);
		}

		for(auto Element: GLTFJson["materials"].get_array())
		{
			auto JsonObject = Element.get_object();
			
			Material MaterialObject{};

			auto PbrObject = JsonObject["pbrMetallicRoughness"].get_object();
			PBRMetallicRoughness PbrMetallicRoughnessObject{};
			if (PbrObject.at_key("baseColorTexture").error() == simdjson::error_code::SUCCESS)
			{
				PbrMetallicRoughnessObject.BaseColorTex.Index = static_cast<int>(PbrObject["baseColorTexture"]["index"].get_int64().take_value());
			}

			if (PbrObject.at_key("metallicRoughnessTexture").error() == simdjson::error_code::SUCCESS)
			{
				PbrMetallicRoughnessObject.MetallicRoughnessTex.Index = static_cast<int>(PbrObject["metallicRoughnessTexture"]["index"].get_int64().take_value());
			}

			if(PbrObject.at_key("baseColorFactor").error() == simdjson::error_code::SUCCESS)
			{
				auto ColorArray = PbrObject["baseColorFactor"].get_array();
				PbrMetallicRoughnessObject.BaseColorFactor.x = static_cast<float>(ColorArray.at(0).get_double().take_value());
				PbrMetallicRoughnessObject.BaseColorFactor.y = static_cast<float>(ColorArray.at(1).get_double().take_value());
				PbrMetallicRoughnessObject.BaseColorFactor.z = static_cast<float>(ColorArray.at(2).get_double().take_value());
			}

			if(PbrObject.at_key("metallicFactor").error() == simdjson::error_code::SUCCESS)
			{
				PbrMetallicRoughnessObject.MetallicFactor = static_cast<float>(PbrObject["metallicFactor"].get_double().take_value());
			}

			if(PbrObject.at_key("roughnessFactor").error() == simdjson::error_code::SUCCESS)
			{
				PbrMetallicRoughnessObject.RoughnessFactor = static_cast<float>(PbrObject["roughnessFactor"].get_double().take_value());
			}

			MaterialObject.ThisMaterialPBRMetallicRoughness = PbrMetallicRoughnessObject;

			Result.Materials.push_back(MaterialObject);
		}

		for (auto Texture : GLTFJson["textures"].get_array())
		{
			auto JsonObject = Texture.get_object();
			TextureInfo TextureObject{};
			TextureObject.Sampler = static_cast<int>(JsonObject["sampler"].get_int64().take_value());
			TextureObject.Source = static_cast<int>(JsonObject["source"].get_int64().take_value());
			if (JsonObject["name"].get_string().error() == simdjson::error_code::SUCCESS)
			{
				TextureObject.Name = JsonObject["name"].get_string().take_value();
			}
			Result.Textures.push_back(TextureObject);
		}

		return Result;
	}
}
