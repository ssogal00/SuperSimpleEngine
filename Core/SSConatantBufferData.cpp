#include "SSCore.h"
#include "SSConstantBufferData.h"

SSConstantBufferData::~SSConstantBufferData()
{
	FreeBufferData();
}

void SSConstantBufferData::FreeBufferData()
{
	if (mpBufferData != nullptr && mBufferSize > 0)
	{
		delete[] mpBufferData;
		mpBufferData = nullptr;
		mBufferSize = 0;
	}
}

SSConstantBufferData::SSConstantBufferData(const SSConstantBufferData& rhs)
{
	if (mBufferSize != rhs.mBufferSize)
	{
		FreeBufferData();
	}

	mBufferSize = rhs.mBufferSize;
	if (mpBufferData == nullptr)
	{
		mpBufferData = new BYTE[mBufferSize];
	}

	memcpy_s(mpBufferData, mBufferSize, rhs.mpBufferData, mBufferSize);
}

SSConstantBufferData::SSConstantBufferData(SSConstantBufferData&& rhs)
{
	if (mBufferSize != rhs.mBufferSize)
	{
		FreeBufferData();
	}

	mBufferSize = rhs.mBufferSize;
	if (mpBufferData == nullptr)
	{
		mpBufferData = new BYTE[mBufferSize];
	}

	memcpy_s(mpBufferData, mBufferSize, rhs.mpBufferData, mBufferSize);

	rhs.FreeBufferData();
}

SSConstantBufferData& SSConstantBufferData::operator = (SSConstantBufferData&& rhs)
{
	if (mBufferSize != rhs.mBufferSize)
	{
		FreeBufferData();
	}

	mBufferSize = rhs.mBufferSize;
	if (mpBufferData == nullptr)
	{
		mpBufferData = new BYTE[mBufferSize];
	}

	memcpy_s(mpBufferData, mBufferSize, rhs.mpBufferData, mBufferSize);

	rhs.FreeBufferData();

	return *this;
}

SSConstantBufferData& SSConstantBufferData::operator=(const SSConstantBufferData& rhs)
{
	if (mBufferSize != rhs.mBufferSize)
	{
		FreeBufferData();
	}

	mBufferSize = rhs.mBufferSize;
	
	if (mpBufferData == nullptr)
	{
		mpBufferData = new BYTE[mBufferSize];
	}

	memcpy_s(mpBufferData, mBufferSize, rhs.mpBufferData, mBufferSize);

	return *this;
}



