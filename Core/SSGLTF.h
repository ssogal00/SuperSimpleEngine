#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <windows.h>
#include <DirectXMath.h>

#include "SSCore.h"

namespace GLTF {
    enum CORE_API ComponentType
    {
        COMPONENT_BYTE = 5120,
        COMPONENT_UNSIGNED_BYTE = 5121,
        COMPONENT_SHORT = 5122,
        COMPONENT_UNSIGNED_SHORT = 5123,
        COMPONENT_UNSIGNED_INT = 5125,
        COMPONENT_FLOAT = 5126,
    };

    enum CORE_API AttributeType
    {
        /// SCALAR
        /// VEC2
        /// VEC3
        /// VEC4
        /// MAT2
        /// MAT3
        /// MAT4
        None,
        SCALAR,
        VEC2,
        VEC3,
        VEC4,
        MAT2,
        MAT3,
        MAT4,
    };
   
    enum CORE_API BufferTarget
    {
        //
        ARRAY_BUFFER = 34962,
        ELEMENT_ARRAY_BUFFER = 34963,
    };

    enum CORE_API AlphaMode
    {
        OPAQUE_MODE,
        MASK_MODE,
        BLEND_MODE,
    };

    class CORE_API Buffer
    {
    public:
        int ByteLength;
        int Type;
        std::string Uri;
    };

    class CORE_API BufferView
    {
    public:
        int64_t Buffer;
        int64_t ByteLength;
        int64_t ByteOffset;
        int64_t Target;
        std::string Name;
        int64_t ByteStride;
    };

    class CORE_API TextureInfo
    {
    public:
        int Sampler;
        int Source;
        std::string Name;
    };


    class CORE_API MinMax
    {

    };

    class CORE_API Attributes
    {
    public:
        int Normal;
        int Position;
        int TexCoord0;
    };

    class CORE_API Accessor
    {
    public:
        int BufferView;
        int ByteOffset;
        GLTF::AttributeType Type;
        int Count;
        GLTF::ComponentType ComponentType;
        std::string Name;
    };

    class CORE_API Primitive
    {
    public:
        int Mode;
        std::map<std::string, int> Attributes;
        int Indices = -1;
        int Material;
    };

    class CORE_API EmissiveTexture 
    {
    public:
        int Index;
    };

    class CORE_API NormalTexture
    {
    public:
        int Index;
    };

    class CORE_API OcclusionTexture
    {
    public:
        int Index;
    };

    class CORE_API BaseColorTexture 
    {
    public:
        int Index;
    };

    class CORE_API MetallicRoughnessTexture
    {
    public:
        int Index;
    };

    class CORE_API PBRMetallicRoughness
    {
    public:
        XMFLOAT3 BaseColorFactor;
        float MetallicFactor;
        float RoughnessFactor;

        MetallicRoughnessTexture MetallicRoughnessTex;
		BaseColorTexture BaseColorTex;
		NormalTexture NormalTex;
		EmissiveTexture EmissiveTex;
    };

    class CORE_API Material
    {
    public:
		bool DoubleSided;
        std::string Name;
		PBRMetallicRoughness ThisMaterialPBRMetallicRoughness;
    };

    class CORE_API Mesh
    {
    public:
        std::string Name;
        std::vector<Primitive> Primitives;
    };

    class CORE_API OpenFileAndSetPointer
    {
    public:
        OpenFileAndSetPointer(const std::string& InPath, int64_t InOffset);
        ~OpenFileAndSetPointer();
        HANDLE GetFileHandle() const {
            return FileHandle; ;
        }

    protected:
        HANDLE FileHandle = INVALID_HANDLE_VALUE;
    };

    class CORE_API GLTFVertexData
    {
    public:
        std::vector<XMFLOAT3> Positions;
        std::vector<XMFLOAT3> Normals;
        std::vector<XMFLOAT4> Tangents;
        std::vector<XMFLOAT2> Texcoords;
		std::vector<uint16_t> Indices;

        unsigned int PostionOffsetInBytes = 0;
        unsigned int NormalOffsetInBytes = 0;
        unsigned int TangentsOffsetInBytes = 0;
        unsigned int TexcoordsOffsetInBytes = 0;
        unsigned int IndexOffsetInBytes = 0;
    };

    class CORE_API SSGLTF_V2
    {
    public:
        static SSGLTF_V2 LoadGLTFFile(const std::string& InPath);

        static std::vector<XMFLOAT3> ParseVector3s(std::string InPath, int64_t InOffset, int64_t InCount, int64_t InByteLength);
		static std::vector<XMFLOAT2> ParseVector2s(std::string InPath, int64_t InOffset, int64_t InCount, int64_t InByteLength);
		static std::vector<XMFLOAT4> ParseVector4s(std::string InPath, int64_t InOffset, int64_t InCount, int64_t InByteLength);
		static std::vector<uint16_t> ParseUInt16s(std::string InPath, int64_t InOffset, int64_t InCount, int64_t InByteLength, uint16_t IndexOffset = 0);
        
    protected:
        std::vector<std::string> Images;
        std::vector<Accessor> Accessors;
        std::vector<BufferView> BufferViews;
		std::vector<Buffer> Buffers;
		std::vector<Mesh> Meshes;

		std::map<std::string, GLTFVertexData> MeshVertexDataMap;

    public:
		// we only need these merged data for creating single big buffers
        std::vector<XMFLOAT3> MergedPositions;
        std::vector<XMFLOAT3> MergedNormals;
		std::vector<XMFLOAT4> MergedTangents;
		std::vector<XMFLOAT2> MergedTexcoords;
		std::vector<uint16_t> MergedIndices;

		std::map<std::string, unsigned int> MeshNameToIndexCount;
		std::map<std::string, unsigned int> MeshNameToPositionCount;
        std::map<std::string, unsigned int> MeshNameToMaterialIndex;
        std::map<unsigned int, Material> MaterialIndexToMaterial;

        std::vector<unsigned int> IndexCountList;
		std::vector<unsigned int> PositionCountList;

        std::vector<TextureInfo> Textures;

    };
};

