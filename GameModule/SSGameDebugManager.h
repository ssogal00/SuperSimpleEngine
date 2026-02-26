#pragma once


class GAMEMODULE_API SSGameDebugManager
{
public:

	void SetGBufferDumpMode(bool bEnable) { bGBufferDumpMode = bEnable; }
	bool IsGBufferDumpMode() const { return bGBufferDumpMode; }

private:
	bool bGBufferDumpMode = false;
};