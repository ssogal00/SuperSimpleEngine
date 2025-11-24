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
		
	template<class T>
	void SetBufferData(const T& value);

	BYTE* GetData() const { return mpBufferData; }

	UINT GetBufferSize() const { return mBufferSize; }
	
protected:

	void FreeBufferData();	
	BYTE*	mpBufferData = nullptr;
	int		mBufferSize = 0;
};


template<class T>
void SSConstantBufferData::SetBufferData(const T& value)
{	
	if(mBufferSize != sizeof(T))
	{
		FreeBufferData();
		mBufferSize = sizeof(T);
		mpBufferData = new BYTE[mBufferSize];
	}

	memcpy_s(mpBufferData, mBufferSize, &value, mBufferSize);
}

template<class T>
SSConstantBufferData::SSConstantBufferData(const T& value)
{	
	SetBufferData(value);
}