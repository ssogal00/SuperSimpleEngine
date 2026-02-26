#include "SSGLTF.h"
#include "simdjson.h"
#include "simdjson/error.h"
#include <filesystem>

namespace GLTF {

	OpenFileAndSetPointer::OpenFileAndSetPointer(const std::string& InPath, int64_t InOffset)
	{
		FileHandle = CreateFileA(InPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (FileHandle == INVALID_HANDLE_VALUE)
		{
			check(false);
		}
		LARGE_INTEGER li;
		li.QuadPart = InOffset;
		SetFilePointerEx(FileHandle, li, NULL, FILE_BEGIN);
	}

	OpenFileAndSetPointer::~OpenFileAndSetPointer()
	{
		if (FileHandle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(FileHandle);
			FileHandle = INVALID_HANDLE_VALUE;
		}
	}

	std::vector<XMFLOAT3> SSGLTF_V2::ParseVector3s(std::string InPath, int64_t InOffset, int64_t InCount, int64_t InByteLength, bool bNormalize)
	{
		std::vector<XMFLOAT3> Result;

		OpenFileAndSetPointer FileReader(InPath, InOffset);

		DWORD BytesRead = 0;
		std::vector<uint8_t> BufferData;
		BufferData.resize(InByteLength);
		
		BOOL ReadResult = ReadFile(FileReader.GetFileHandle(), 
			BufferData.data(), static_cast<DWORD>(InByteLength), &BytesRead, NULL);
		check(ReadResult == TRUE);

		Result.resize(InCount);

		const uint8_t* DataPtr = BufferData.data();

		for (int i = 0; i < InCount; ++i)
		{
			float X = *reinterpret_cast<const float*>(DataPtr + i * 12 + 0);
			float Y = *reinterpret_cast<const float*>(DataPtr + i * 12 + 4);
			float Z = *reinterpret_cast<const float*>(DataPtr + i * 12 + 8);
			if (bNormalize)
			{
				XMVECTOR Vec = XMVectorSet(X, Y, Z, 0.0f);
				Vec = XMVector3Normalize(Vec);
				Result[i] = XMFLOAT3(Vec.m128_f32[0], Vec.m128_f32[1], Vec.m128_f32[2]);
			}
			else
			{
				Result[i] = XMFLOAT3(X, Y, Z);
			}
			
			
		}
		
		return Result;
	}

	std::vector<XMFLOAT2> SSGLTF_V2::ParseVector2s(std::string InPath, int64_t InOffset, int64_t InCount, int64_t InByteLength)
	{
		std::vector<XMFLOAT2> Result;

		OpenFileAndSetPointer FileReader(InPath, InOffset);

		DWORD BytesRead = 0;
		std::vector<uint8_t> BufferData;
		BufferData.resize(InByteLength);

		BOOL ReadResult = ReadFile(FileReader.GetFileHandle(),
			BufferData.data(), static_cast<DWORD>(InByteLength), &BytesRead, NULL);
		check(ReadResult == TRUE);

		Result.resize(InCount);

		const uint8_t* DataPtr = BufferData.data();

		for (int i = 0; i < InCount; ++i)
		{
			float X = *reinterpret_cast<const float*>(DataPtr + i * 8 + 0);
			float Y = *reinterpret_cast<const float*>(DataPtr + i * 8 + 4);
			Result[i] = XMFLOAT2(X, Y);
		}

		return Result;
	}

	std::vector<uint16_t> SSGLTF_V2::ParseUInt16s(std::string InPath, int64_t InOffset, int64_t InCount, int64_t InByteLength, uint16_t IndexOffset)
	{
		std::vector<uint16_t> Result;
		OpenFileAndSetPointer FileReader(InPath, InOffset);
		DWORD BytesRead = 0;
		std::vector<uint8_t> BufferData;
		BufferData.resize(InByteLength);
		BOOL ReadResult = ReadFile(FileReader.GetFileHandle(),
			BufferData.data(), static_cast<DWORD>(InByteLength), &BytesRead, NULL);
		check(ReadResult == TRUE);
		Result.resize(InCount);
		const uint8_t* DataPtr = BufferData.data();
		for (int i = 0; i < InCount; ++i)
		{
			uint16_t Value = *reinterpret_cast<const uint16_t*>(DataPtr + i * 2);
			Result[i] = Value + IndexOffset;
		}
		return Result;
	}

	std::vector<XMFLOAT4> SSGLTF_V2::ParseVector4s(std::string InPath, int64_t InOffset, int64_t InCount, int64_t InByteLength)
	{
		std::vector<XMFLOAT4> Result;

		OpenFileAndSetPointer FileReader(InPath, InOffset);

		DWORD BytesRead = 0;
		std::vector<uint8_t> BufferData;
		BufferData.resize(InByteLength);

		BOOL ReadResult = ReadFile(FileReader.GetFileHandle(),
			BufferData.data(), static_cast<DWORD>(InByteLength), &BytesRead, NULL);
		check(ReadResult == TRUE);

		Result.resize(InCount);

		const uint8_t* DataPtr = BufferData.data();

		for (int i = 0; i < InCount; ++i)
		{
			float X = *reinterpret_cast<const float*>(DataPtr + i * 16 + 0);
			float Y = *reinterpret_cast<const float*>(DataPtr + i * 16 + 4);
			float Z = *reinterpret_cast<const float*>(DataPtr + i * 16 + 8);
			float W = *reinterpret_cast<const float*>(DataPtr + i * 16 + 12);
			Result[i] = XMFLOAT4(X, Y, Z, W);
		}

		return Result;
	}

	SSGLTF_V2 SSGLTF_V2::LoadGLTFFile(const std::string& InPath)
	{
		simdjson::dom::parser Parser;
		simdjson::dom::element GLTFJson = Parser.load(InPath);

		SSGLTF_V2 Result{};

#pragma region ParseGLTF
		for(auto Element: GLTFJson["meshes"].get_array())
		{
			auto JsonObject = Element.get_object();
			Mesh MeshObject{};
			MeshObject.Name = JsonObject["name"].get_string().take_value();
			for (auto PrimitiveElement : JsonObject["primitives"].get_array())
			{
				auto PrimitiveObject = PrimitiveElement.get_object();
				Primitive Prim{};
				auto AttributesObject = PrimitiveObject["attributes"].get_object();
				for (auto Attribute : AttributesObject)
				{
					auto Key = Attribute.key;
					auto Value = static_cast<int>(Attribute.value.get_int64().take_value());
					Prim.Attributes[std::string(Key)] = Value;
				}

				if (PrimitiveObject.at_key("indices").error() == simdjson::error_code::SUCCESS)
				{
					Prim.Indices = static_cast<int>(PrimitiveObject["indices"].get_int64().take_value());
				}
				else
				{
					Prim.Indices = -1;
				}


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
			if(JsonObject.at_key("byteOffset").error() == simdjson::error_code::SUCCESS)
			{
				AccessorObject.ByteOffset = static_cast<int>(JsonObject["byteOffset"].get_int64().take_value());
			}
			else
			{
				AccessorObject.ByteOffset = 0;
			}
			
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
			if(JsonObject.at_key("byteOffset").error() == simdjson::error_code::SUCCESS)
			{
				View.ByteOffset = JsonObject["byteOffset"].get_int64().take_value();
			}
			else
			{
				View.ByteOffset = 0;
			}
			
			if(JsonObject.at_key("byteStride").error() == simdjson::error_code::SUCCESS)
			{
				View.ByteStride = JsonObject["byteStride"].get_int64().take_value();
			}
			else
			{
				View.ByteStride = 0;
			}
			if (JsonObject.at_key("target").error() == simdjson::error_code::SUCCESS)
			{
				View.Target = JsonObject["target"].get_int64().take_value();
			}
			else
			{
				View.Target = 0;
			}
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

		unsigned int MaterialIndex = 0;
		for(auto Element: GLTFJson["materials"].get_array())
		{
			auto JsonObject = Element.get_object();
			
			Material MaterialObject{};

			if(JsonObject.at_key("normalTexture").error() == simdjson::error_code::SUCCESS)
			{
				MaterialObject.NormalTex.Index = static_cast<int>(JsonObject["normalTexture"]["index"].get_int64().take_value());
			}

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

			Result.MaterialIndexToMaterial[MaterialIndex++] = MaterialObject;
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

#pragma endregion


		unsigned int CurrentPositionOffset = 0;
		unsigned int CurrentNormalOffset = 0;
		unsigned int CurrentTangentOffset = 0;
		unsigned int CurrentTexcoordOffset = 0;
		unsigned int CurrentIndexOffset = 0;
		unsigned int CurrentIndexCount = 0;

		for (auto& Mesh : Result.Meshes)
		{	
			for (auto& Primitive : Mesh.Primitives)
			{
				std::string BasePath = std::filesystem::path(InPath).parent_path().string();

				std::string MeshKey = Mesh.Name;

				if(Primitive.Indices != -1)
				{
					int AccessorIndex = Primitive.Indices;
					Accessor& IndexAccessor = Result.Accessors[AccessorIndex];
					BufferView& IndexBufferView = Result.BufferViews[IndexAccessor.BufferView];
					Buffer& IndexBuffer = Result.Buffers[IndexBufferView.Buffer];
					IndexBuffer.Uri;
					std::string UriPath = BasePath + "/" + IndexBuffer.Uri;
					std::vector<uint16_t> Indices;

					int64_t IndexByteOffset = IndexAccessor.ByteOffset + IndexBufferView.ByteOffset;
					int64_t Length = IndexAccessor.Count * 2;

					uint16_t IndexOffset = static_cast<uint16_t>(Result.MergedPositions.size());

					Indices = ParseUInt16s(UriPath, IndexByteOffset, IndexAccessor.Count, Length, IndexOffset);

					Result.MeshVertexDataMap[MeshKey].Indices = Indices;

					Result.MergedIndices.insert(Result.MergedIndices.end(),
						Indices.begin(),
						Indices.end());

					Result.IndexCountList.push_back(Indices.size());

					Result.MeshNameToIndexCount[MeshKey] = static_cast<unsigned int>(Indices.size());
				}

				if(Primitive.Material != -1)
				{
					Result.MeshNameToMaterialIndex[MeshKey] = Primitive.Material;
				}

				// Load Positions
				if (Primitive.Attributes.find("POSITION") != Primitive.Attributes.end())
				{
					int AccessorIndex = Primitive.Attributes["POSITION"];
					Accessor& PositionAccessor = Result.Accessors[AccessorIndex];
					BufferView& PositionBufferView = Result.BufferViews[PositionAccessor.BufferView];
					Buffer& PositionBuffer = Result.Buffers[PositionBufferView.Buffer];
					PositionBuffer.Uri;
					std::string UriPath = BasePath + "/" + PositionBuffer.Uri;

					int64_t PositionByteOffset = PositionAccessor.ByteOffset + PositionBufferView.ByteOffset;

					std::vector<XMFLOAT3> PositionList;
					check(PositionBufferView.ByteStride > 0);
					int64_t ByteLength = PositionAccessor.Count * PositionBufferView.ByteStride;
					PositionList = ParseVector3s(UriPath, PositionByteOffset, PositionAccessor.Count, ByteLength);

					Result.MeshVertexDataMap[MeshKey].Positions = PositionList;

					Result.PositionCountList.push_back(static_cast<unsigned int>(PositionList.size()));

					Result.MergedPositions.insert(Result.MergedPositions.end(),
						PositionList.begin(),
						PositionList.end());

					Result.MeshNameToPositionCount[MeshKey] = static_cast<unsigned int>(PositionList.size());
				}
				// Parse Normals
				if (Primitive.Attributes.find("NORMAL") != Primitive.Attributes.end())
				{
					int AccessorIndex = Primitive.Attributes["NORMAL"];
					Accessor& NormalAccessor = Result.Accessors[AccessorIndex];
					BufferView& NormalBufferView = Result.BufferViews[NormalAccessor.BufferView];
					Buffer& NormalBuffer = Result.Buffers[NormalBufferView.Buffer];
					NormalBuffer.Uri;
					std::string UriPath = BasePath + "/" + NormalBuffer.Uri;

					int64_t NormalByteOffset = NormalBufferView.ByteOffset + NormalAccessor.ByteOffset;
					int64_t ByteLength = NormalBufferView.ByteStride * NormalAccessor.Count;
					
					Result.MeshVertexDataMap[MeshKey].Normals = ParseVector3s(UriPath, NormalByteOffset, NormalAccessor.Count, ByteLength);
					
					Result.MergedNormals.insert(Result.MergedNormals.end(),
						Result.MeshVertexDataMap[MeshKey].Normals.begin(),
						Result.MeshVertexDataMap[MeshKey].Normals.end());
				}
				// Parse Tangents
				if (Primitive.Attributes.find("TANGENT") != Primitive.Attributes.end())
				{
					int AccessorIndex = Primitive.Attributes["TANGENT"];
					Accessor& TangentAccessor = Result.Accessors[AccessorIndex];
					BufferView& TangentBufferView = Result.BufferViews[TangentAccessor.BufferView];
					Buffer& TangentBuffer = Result.Buffers[TangentBufferView.Buffer];
					TangentBuffer.Uri;
					std::string UriPath = BasePath + "/" + TangentBuffer.Uri;

					int64_t TangentByteOffset = TangentBufferView.ByteOffset + TangentAccessor.ByteOffset;
					int64_t ByteLength = TangentAccessor.Count * TangentBufferView.ByteStride;

					Result.MeshVertexDataMap[MeshKey].Tangents = ParseVector4s(UriPath, TangentByteOffset, TangentAccessor.Count, ByteLength);

					Result.MergedTangents.insert(Result.MergedTangents.end(),
						Result.MeshVertexDataMap[MeshKey].Tangents.begin(),
						Result.MeshVertexDataMap[MeshKey].Tangents.end());
				}

				if (Primitive.Attributes.find("TEXCOORD_0") != Primitive.Attributes.end())
				{
					int AccessorIndex = Primitive.Attributes["TEXCOORD_0"];
					Accessor& TexcoordAccessor = Result.Accessors[AccessorIndex];
					BufferView& TexcoordBufferView = Result.BufferViews[TexcoordAccessor.BufferView];
					Buffer& TexcoordBuffer = Result.Buffers[TexcoordBufferView.Buffer];
					TexcoordBuffer.Uri;
					std::string UriPath = BasePath + "/" + TexcoordBuffer.Uri;

					int64_t TexcoordByteOffset = TexcoordBufferView.ByteOffset + TexcoordAccessor.ByteOffset;

					Result.MeshVertexDataMap[MeshKey].Texcoords = ParseVector2s(UriPath, TexcoordByteOffset, TexcoordAccessor.Count, TexcoordBufferView.ByteLength);

					Result.MergedTexcoords.insert(Result.MergedTexcoords.end(),
						Result.MeshVertexDataMap[MeshKey].Texcoords.begin(),
						Result.MeshVertexDataMap[MeshKey].Texcoords.end());
				}
					
			}
			
		}

		return Result;
	}
}
