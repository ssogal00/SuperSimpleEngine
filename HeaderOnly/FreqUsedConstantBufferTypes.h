#pragma once

#include "DirectXMath.h"


struct Transform
{
	DirectX::XMFLOAT4X4 Model;
	DirectX::XMFLOAT4X4 View;
	DirectX::XMFLOAT4X4 Proj;
};

struct MVP
{
	DirectX::XMMATRIX ModelViewProj;
};

 struct alignas(16) CbufferFloat
{
	float value;
};

 struct alignas(16) CbufferUnsingedInt
 {
	 unsigned int Value;
 };

 struct alignas(16) CbufferInvScreenSize
 {
	 XMFLOAT2 InvScreenSize;
 };

 template<class... T>
struct alignas(16) SSAlignedCBuffer
 {	 
 };

template<class T1>
struct alignas(16) SSAlignedCBuffer<T1>
{
	T1 value1;	
};


 template<class T1, class T2>
 struct alignas(16) SSAlignedCBuffer<T1,T2>
 {
	 T1 value1;
	 T2 value2;
 };

 template<class T1, class T2, class T3>
 struct alignas(16) SSAlignedCBuffer<T1,T2,T3>
 {
	 T1 value1;
	 T2 value2;
	 T3 value3;
 };

 template<class T1, class T2, class T3, class T4>
 struct alignas(16) SSAlignedCBuffer<T1,T2,T3,T4>
 {
	 T1 value1;
	 T2 value2;
	 T3 value3;
	 T4 value4;
 };

 template<class T1, class T2, class T3, class T4, class T5>
 struct alignas(16) SSAlignedCBuffer<T1, T2, T3, T4, T5>
 {
	 T1 value1;
	 T2 value2;
	 T3 value3;
	 T4 value4;
	 T5 value5;
 };

 struct alignas(16) TextureStreamingInfoCB
 {
     XMFLOAT4 DiffuseStreamInfo;
     XMFLOAT4 NormalStreamInfo;
     XMFLOAT4 RoughnessStreamInfo;
     XMFLOAT4 MetalicStreamInfo;
 };