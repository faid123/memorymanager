#pragma once

#include "Hash.h"
#include "MemoryManager.h"

const int MEMORY_ALIGNMENT = 16;
const int KB_TO_BYTE = 1024;
const int KB_TO_MB = 1024;

class GlobalManager
{
public:
	GlobalManager() = default;
	~GlobalManager() = delete;
	GlobalManager& operator=(const GlobalManager& source) = delete;
	GlobalManager(const GlobalManager& source) = delete;
	GlobalManager& operator=(GlobalManager&&) = delete;
	GlobalManager(const GlobalManager&&) = delete;

	static MemoryManager* GetMemoryManager();
	static Hash* GetHash();

private:
	static MemoryManager memoryManager;
	static Hash hash;
};