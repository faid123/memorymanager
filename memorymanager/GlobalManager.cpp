#include "GlobalManager.h"

MemoryManager GlobalManager::memoryManager;
Hash GlobalManager::hash;

MemoryManager* GlobalManager::GetMemoryManager()
{
	return &memoryManager;
}

Hash* GlobalManager::GetHash()
{
	return &hash;
}
