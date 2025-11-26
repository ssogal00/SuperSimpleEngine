#pragma once

#include "SSCore.h"

class CORE_API SSConstantBufferData
{
public:
	SSConstantBufferData(){}
	
	template<class T>
	SSConstantBufferData(const T& value);

	SSConstantBufferData(const SSConstantBufferData& rhs);

	SSConstantBufferData(SSConstantBufferData&& rhs);

	SSConstantBufferData& operator=(SSConstantBufferData&& rhs);

	SSConstantBufferData& operator=(const SSConstantBufferData& rhs);
	
	~SSConstantBufferData();

	void CopyFrom(const SSConstantBufferData& rhs);
		
	template<class T>
	void SetBufferData(const T& value);

	BYTE* GetData() const { return mpBufferData; }

	template<class T>
	T* GetDataPtrAs() const;

	template<class T>
	T GetDataAs() const;

	UINT GetBufferSize() const { return mBufferSize; }
	
protected:

	void FreeBufferData();	
	BYTE*	mpBufferData = nullptr;
	int		mBufferSize = 0;
	int		GetAlignedBufferSize(int size) const { return (size + 15) & ~15; }
};


template<class T>
void SSConstantBufferData::SetBufferData(const T& value)
{	
	if(mBufferSize != sizeof(T))
	{
		FreeBufferData();
		mBufferSize = GetAlignedBufferSize(sizeof(T));
		mpBufferData = new BYTE[mBufferSize];
	}

	memcpy_s(mpBufferData, mBufferSize, &value, mBufferSize);
}

template<class T>
T* SSConstantBufferData::GetDataPtrAs() const
{
	check(mBufferSize == GetAlignedBufferSize(sizeof(T)));
	return reinterpret_cast<T*>(mpBufferData);
}

template<class T>
SSConstantBufferData::SSConstantBufferData(const T& value)
{	
	SetBufferData(value);
}

template<class T>
T SSConstantBufferData::GetDataAs() const
{
	check(mBufferSize == GetAlignedBufferSize(sizeof(T)));
	return *reinterpret_cast<T*>(mpBufferData);
}