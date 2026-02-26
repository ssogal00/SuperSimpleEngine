#pragma once


class GAMEMODULE_API SSGameDebugManager
{
public:

	void SetGBufferDumpMode(bool bEnable) { bGBufferDumpMode = bEnable; }
	bool IsGBufferDumpMode() const { return bGBufferDumpMode; }
	static SSGameDebugManager& Get();
private:
	bool bGBufferDumpMode = false;
	static SSGameDebugManager* mInstance;
};