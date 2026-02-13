

#include "SSCore.h"
#include "SSSharedRenderData.h"

using namespace DirectX;


void SSSharedRenderData::Initialize()
{
	CreateCubeVertexData();
	CreateSphereVertexData();
	CreateScreenBlitVertexData();
}

SSSimpleVertexData SSSharedRenderData::GetCubeVertexData()
{
	SSSimpleVertexData Result{};
	Result.Count = mCubeVertexData.size();
	Result.DataPtr = mCubeVertexData.data();
	Result.Stride = sizeof(VT_PositionNormalTexcoordTangent);

	return Result;
}

SSSimpleVertexData SSSharedRenderData::GetSphereVertexData()
{
	SSSimpleVertexData Result{};
	Result.Count = mSphereVertexData.size();
	Result.DataPtr = mSphereVertexData.data();
	Result.Stride = sizeof(VT_PositionNormalTexcoordTangent);

	return Result;
}

SSSimpleVertexData SSSharedRenderData::GetScreenBlitVertexData()
{
	SSSimpleVertexData Result{};
	Result.Count = mScreenBlitVertexData.size();
	Result.Stride = sizeof(VT_PositionTexcoord);
	Result.DataPtr = mScreenBlitVertexData.data();

	return Result;
}
void SSSharedRenderData::CreateCubeVertexData()
{
	mCubeVertexData = std::vector<VT_PositionNormalTexcoordTangent>
	{
		// front
		{ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1), XMFLOAT3(0, 0, -1), XMFLOAT2(0, 0), XMFLOAT4(0, 0, -1, 1)},
		{ XMFLOAT4(-1.0f, +1.0f, -1.0f,1), XMFLOAT3(0,0,-1),  XMFLOAT2(0,1),XMFLOAT4(0,0,-1,1) },
		{ XMFLOAT4(+1.0f, +1.0f, -1.0f,1), XMFLOAT3(0,0,-1),   XMFLOAT2(1,1) ,XMFLOAT4(0,0,-1,1) },
		{ XMFLOAT4(+1.0f, -1.0f, -1.0f,1),  XMFLOAT3(0,0,-1),   XMFLOAT2(1,0) ,XMFLOAT4(0,0,-1,1) },

		// back
		{ XMFLOAT4(-1.0f, -1.0f, +1.0f,1),  XMFLOAT3(0,0,+1),   XMFLOAT2(0,0) ,XMFLOAT4(0,0,1,1) },
		{ XMFLOAT4(-1.0f, +1.0f, +1.0f,1),  XMFLOAT3(0,0,+1),   XMFLOAT2(0,1) ,XMFLOAT4(0,0,1,1) },
		{ XMFLOAT4(+1.0f, +1.0f, +1.0f,1),  XMFLOAT3(0,0,+1),   XMFLOAT2(1,1) ,XMFLOAT4(0,0,1,1) },
		{ XMFLOAT4(+1.0f, -1.0f, +1.0f,1),  XMFLOAT3(0,0,+1),   XMFLOAT2(1,0) ,XMFLOAT4(0,0,1,1) },

			// top
		{ XMFLOAT4(-1.0f, +1.0f, +1.0f, 1), XMFLOAT3(0,1,0), XMFLOAT2(0,0),XMFLOAT4(0,1,0,1) },
		{ XMFLOAT4(1.0f, +1.0f, +1.0f, 1), XMFLOAT3(0,1,0), XMFLOAT2(1,0),XMFLOAT4(0,1,0,1) },
		{ XMFLOAT4(1.0f, +1.0f, -1.0f, 1), XMFLOAT3(0,1,0), XMFLOAT2(1,1),XMFLOAT4(0,1,0,1) },
		{ XMFLOAT4(-1.0f, +1.0f, -1.0f, 1), XMFLOAT3(0,1,0), XMFLOAT2(0,1),XMFLOAT4(0,1,0,1) },


		// bottom
		{ XMFLOAT4(-1.0f,-1.0f, +1.0f, 1), XMFLOAT3(0,-1,0), XMFLOAT2(0,0),XMFLOAT4(0,-1,0,1) },
		{ XMFLOAT4(1.0f, -1.0f, +1.0f, 1), XMFLOAT3(0,-1,0), XMFLOAT2(1,0),XMFLOAT4(0,-1,0,1) },
		{ XMFLOAT4(1.0f, -1.0f, -1.0f, 1), XMFLOAT3(0,-1,0), XMFLOAT2(1,1),XMFLOAT4(0,-1,0,1) },
		{ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1), XMFLOAT3(0,-1,0), XMFLOAT2(0,1),XMFLOAT4(0,-1,0,1) },

		// left
		{ XMFLOAT4(-1.0f, +1.0f, +1.0f, 1), XMFLOAT3(-1,0,0), XMFLOAT2(0,0),XMFLOAT4(-1,0,0,1) },
		{ XMFLOAT4(-1.0f, +1.0f, -1.0f, 1), XMFLOAT3(-1,0,0), XMFLOAT2(1,0),XMFLOAT4(-1,0,0,1) },
		{ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1), XMFLOAT3(-1,0,0), XMFLOAT2(1,1),XMFLOAT4(-1,0,0,1) },
		{ XMFLOAT4(-1.0f, -1.0f, +1.0f, 1), XMFLOAT3(-1,0,0), XMFLOAT2(0,1),XMFLOAT4(-1,0,0,1) },

		// right
		{ XMFLOAT4(+1.0f, +1.0f, -1.0f, 1), XMFLOAT3(1,0,0), XMFLOAT2(0,0),XMFLOAT4(1,0,0,1) },
		{ XMFLOAT4(+1.0f, +1.0f, +1.0f, 1), XMFLOAT3(1,0,0), XMFLOAT2(1,0),XMFLOAT4(1,0,0,1) },
		{ XMFLOAT4(+1.0f, -1.0f, +1.0f, 1), XMFLOAT3(1,0,0), XMFLOAT2(1,1),XMFLOAT4(1,0,0,1) },
		{ XMFLOAT4(+1.0f, -1.0f, -1.0f, 1), XMFLOAT3(1,0,0), XMFLOAT2(0,1),XMFLOAT4(1,0,0,1) },
	};

	mCubeIndexData = std::vector<UINT>
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// top
		8,9,11,
		11,9,10,

		// bottom
		14,13,12,
		15,14,12,

		// left
		16,17,18,
		16,18,19,

		// right
		20,21,22,
		20,22,23,
	};
}

void SSSharedRenderData::CreateSphereVertexData()
{
	const UINT stackCount = 25;
	const float radius = 10;
	const UINT sectorCount = 25;


	// temp 
	std::vector<XMFLOAT4> tempVertexList;
	std::vector<XMFLOAT3> tempNormalList;
	std::vector<XMFLOAT2> tempTexCoordList;
	std::vector<XMFLOAT4> tempTangentList;

	std::vector< VT_PositionNormalTexcoordTangent > vertexArray;	

	UINT CurrentIndex = 0;
	
	for (UINT i = 0; i < stackCount; ++i)
	{
		auto degree1 = (180.0f / (float)stackCount) * (float)i;
		auto degree2 = (180.0f / (float)stackCount) * (float)(i + 1);

		auto sectorRadius1 = radius * DirectX::XMScalarSin(XMConvertToRadians(degree1));
		auto sectorRadius2 = radius * DirectX::XMScalarSin(XMConvertToRadians(degree2));

		auto y1 = radius * DirectX::XMScalarCos(XMConvertToRadians(degree1));
		auto y2 = radius * DirectX::XMScalarCos(XMConvertToRadians(degree2));

		auto v1 = (float)i / (float)stackCount;
		auto v2 = (float)(i + 1) / (float)stackCount;

		for (UINT j = 0; j < sectorCount; ++j)
		{
			auto deg1 = (360 / (float)sectorCount) * j;
			auto deg2 = (360 / (float)sectorCount) * (j + 1);

			auto u1 = (float)j / (float)sectorCount;
			auto u2 = (float)(j + 1) / (float)sectorCount;

			// v1
			auto x1 = sectorRadius1 * DirectX::XMScalarCos(XMConvertToRadians(deg1));
			auto z1 = sectorRadius1 * DirectX::XMScalarSin(XMConvertToRadians(deg1));

			// v2
			auto x2 = sectorRadius1 * DirectX::XMScalarCos(XMConvertToRadians(deg2));
			auto z2 = sectorRadius1 * DirectX::XMScalarSin(XMConvertToRadians(deg2));

			// v3
			auto x3 = sectorRadius2 * DirectX::XMScalarCos(XMConvertToRadians(deg1));
			auto z3 = sectorRadius2 * DirectX::XMScalarSin(XMConvertToRadians(deg1));

			// v4
			auto x4 = sectorRadius2 * DirectX::XMScalarCos(XMConvertToRadians(deg2));
			auto z4 = sectorRadius2 * DirectX::XMScalarSin(XMConvertToRadians(deg2));

			auto V1 = XMFLOAT4{ (float)x1, (float)y1, (float)z1 , 1 };
			auto T1 = XMFLOAT2(u1, v1);

			auto V2 = XMFLOAT4((float)x2, (float)y1, (float)z2, 1);
			auto T2 = XMFLOAT2(u2, v1);

			auto V3 = XMFLOAT4((float)x3, (float)y2, (float)z3, 1);
			auto T3 = XMFLOAT2(u1, v2);

			auto V4 = XMFLOAT4((float)x4, (float)y2, (float)z4, 1);
			auto T4 = XMFLOAT2(u2, v2);


			XMFLOAT3 norm1; XMStoreFloat3(&norm1, XMVector3Normalize(XMLoadFloat4(&V1)));
			XMFLOAT3 norm2; XMStoreFloat3(&norm2, XMVector3Normalize(XMLoadFloat4(&V2)));
			XMFLOAT3 norm3; XMStoreFloat3(&norm3, XMVector3Normalize(XMLoadFloat4(&V3)));
			XMFLOAT3 norm4; XMStoreFloat3(&norm4, XMVector3Normalize(XMLoadFloat4(&V4)));

			// V1-----V2
			//       /
			//      /
			//     /
			//    /
			//   /
			//  /
			// V3-----V4


			UINT V1Index = CurrentIndex++;
			UINT V2Index = CurrentIndex++;
			UINT V3Index = CurrentIndex++;
			UINT V4Index = CurrentIndex++;

			tempVertexList.push_back(V1);
			tempVertexList.push_back(V2);
			tempVertexList.push_back(V3);

			tempVertexList.push_back(V3);
			tempVertexList.push_back(V2);
			tempVertexList.push_back(V4);

			mSphereIndexData.push_back(V1Index);
			mSphereIndexData.push_back(V2Index);
			mSphereIndexData.push_back(V3Index);

			mSphereIndexData.push_back(V3Index);
			mSphereIndexData.push_back(V2Index);
			mSphereIndexData.push_back(V4Index);

			tempNormalList.push_back(norm1);
			tempNormalList.push_back(norm2);
			tempNormalList.push_back(norm3);

			tempNormalList.push_back(norm3);
			tempNormalList.push_back(norm2);
			tempNormalList.push_back(norm4);

			tempTexCoordList.push_back(T1);
			tempTexCoordList.push_back(T2);
			tempTexCoordList.push_back(T3);

			tempTexCoordList.push_back(T3);
			tempTexCoordList.push_back(T2);
			tempTexCoordList.push_back(T4);
		}
	}

	tempTangentList = GenerateTangents(tempVertexList, tempNormalList, tempTexCoordList);


	std::vector<UINT> indexArray;

	for (UINT i = 0; i < tempVertexList.size(); i += 3)
	{
		// face1
		mSphereVertexData.push_back(VT_PositionNormalTexcoordTangent
		(
			tempVertexList[i], tempNormalList[i], tempTexCoordList[i], tempTangentList[i]
		));

		mSphereVertexData.push_back(VT_PositionNormalTexcoordTangent
		(
			tempVertexList[i + 1], tempNormalList[i + 1], tempTexCoordList[i + 1], tempTangentList[i + 1]
		));

		mSphereVertexData.push_back(VT_PositionNormalTexcoordTangent
		(
			tempVertexList[i + 2], tempNormalList[i + 2], tempTexCoordList[i + 2], tempTangentList[i + 2]
		));
	}
}

void SSSharedRenderData::CreateScreenBlitVertexData()
{
	std::vector<VT_PositionTexcoord> vertexArray
	{
		{XMFLOAT4(-1,1,0,1), XMFLOAT2(0,0)},
		{XMFLOAT4(1,1,0,1), XMFLOAT2(1,0)},
		{XMFLOAT4(1,-1,0,1), XMFLOAT2(1,1)},
		{XMFLOAT4(-1,-1,0,1), XMFLOAT2(0,1)},
	};

	mScreenBlitVertexData = std::move(vertexArray);

	std::vector<UINT> indexArray
	{
		0,1,2,
		0,2,3,
	};

	mScreenBlitIndexData = std::move(indexArray);
}

std::vector<XMFLOAT4> SSSharedRenderData::GenerateTangents(const std::vector<XMFLOAT4>& tempVertexList, 
                                                           const std::vector<XMFLOAT3>& tempNormalList,
                                                           const std::vector<XMFLOAT2>& tempTexCoordList)
{
	std::vector<XMFLOAT4> mTempTangentList;
	
	std::vector<XMVECTOR> tan1Accum;
	std::vector<XMVECTOR> tan2Accum;

	for (UINT i = 0; i < tempVertexList.size(); ++i)
	{
		tan1Accum.push_back(XMVECTOR());
		tan2Accum.push_back(XMVECTOR());
	}

	for (UINT i = 0; i < tempVertexList.size(); i++)
	{
		mTempTangentList.push_back(XMFLOAT4(0, 0, 0, 0));
	}

	// Compute the tangent vector
	for (UINT i = 0; i < tempVertexList.size(); i += 3)
	{
		XMVECTOR p1 = XMLoadFloat4(&tempVertexList[(int)i]);
		XMVECTOR p2 = XMLoadFloat4(&tempVertexList[(int)i + 1]);
		XMVECTOR p3 = XMLoadFloat4(&tempVertexList[(int)i + 2]);

		XMVECTOR tc1 = XMLoadFloat2(&tempTexCoordList[(int)i]);
		XMVECTOR tc2 = XMLoadFloat2(&tempTexCoordList[(int)i + 1]);
		XMVECTOR tc3 = XMLoadFloat2(&tempTexCoordList[(int)i + 2]);

		XMFLOAT3 q1; XMStoreFloat3(&q1, p2 - p1);
		XMFLOAT3 q2; XMStoreFloat3(&q2, p3 - p1);

		float s1 = tempTexCoordList[(int)i + 1].x - tempTexCoordList[(int)i].x;
		float s2 = tempTexCoordList[(int)i + 2].x - tempTexCoordList[(int)i].x;

		float t1 = tempTexCoordList[(int)i + 1].y - tempTexCoordList[(int)i].y;
		float t2 = tempTexCoordList[(int)i + 2].y - tempTexCoordList[(int)i].y;

		// prevent degeneration
		float r = 1.0f / (s1 * t2 - s2 * t1);

		if (std::isinf(r))
		{
			r = 1 / 0.1f;
		}

		XMFLOAT3 tmptan1;
		tmptan1.x = (t2 * q1.x - t1 * q2.x) * r;
		tmptan1.y = (t2 * q1.y - t1 * q2.y) * r;
		tmptan1.z = (t2 * q1.z - t1 * q2.z) * r;

		XMFLOAT3 tmptan2;
		tmptan2.x = (s1 * q2.x - s2 * q1.x) * r;
		tmptan2.y = (s1 * q2.y - s2 * q1.y) * r;
		tmptan2.z = (s1 * q2.z - s2 * q1.z) * r;

		auto tan1 = XMLoadFloat3(&tmptan1);
		auto tan2 = XMLoadFloat3(&tmptan2);

		tan1Accum[(int)i] += tan1;
		tan1Accum[(int)i + 1] += tan1;
		tan1Accum[(int)i + 2] += tan1;

		tan2Accum[(int)i] += tan2;
		tan2Accum[(int)i + 1] += tan2;
		tan2Accum[(int)i + 2] += tan2;
	}

	XMFLOAT4 lastValidTangent(0, 0, 0, 0);

	for (UINT i = 0; i < tempVertexList.size(); ++i)
	{
		auto n = DirectX::XMLoadFloat3(&tempNormalList[(int)i]);
		auto t1 = tan1Accum[(int)i];
		auto t2 = tan2Accum[(int)i];

		// Gram-Schmidt orthogonalize                
		XMFLOAT3 temp;
		//XMStoreFloat3(&temp, XMVector3Normalize(t1 - (XMVector3Dot(n, t1) * n)));

		XMFLOAT3 normalDotTangent;
		XMStoreFloat3(&normalDotTangent, XMVector3Dot(n, t1));

		XMVECTOR computeResult = t1 - XMVectorScale(n, normalDotTangent.x);
		XMFLOAT4 errCheck;
		XMStoreFloat4(&errCheck, computeResult);

		bool bValid = true;

		if (errCheck.x == 0 && errCheck.y == 0 && errCheck.z == 0 && errCheck.w == 0)
		{
			bValid = false;
		}

		XMStoreFloat3(&temp, XMVector3Normalize(computeResult));

		// Store handedness in w                
		XMFLOAT3 dotResult;
		XMStoreFloat3(&dotResult, XMVector3Dot(XMVector3Cross(n, t1), t2));

		auto W = (dotResult.x < 0.0f) ? -1.0f : 1.0f;

		if (std::isnan(temp.x) || std::isnan(temp.y) || std::isnan(temp.z))
		{
			bValid = false;
		}

		if (std::isinf(temp.x) || std::isinf(temp.y) || std::isinf(temp.z))
		{
			bValid = false;
		}

		if (bValid == true)
		{
			lastValidTangent.x = temp.x;
			lastValidTangent.y = temp.y;
			lastValidTangent.z = temp.z;
			lastValidTangent.w = W;
		}

		if (bValid == false)
		{
			temp.x = lastValidTangent.x;
			temp.y = lastValidTangent.y;
			temp.z = lastValidTangent.z;
		}

		mTempTangentList[i].x = temp.x;
		mTempTangentList[i].y = temp.y;
		mTempTangentList[i].z = temp.z;
		mTempTangentList[i].w = W;
	}

	tan1Accum.clear();

	return std::move(mTempTangentList);
}
