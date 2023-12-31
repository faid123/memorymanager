#pragma once
#include "MemoryHelper.h"
#include "MemoryBlock.h"

class MemoryPool : public MemoryHelper
{
public:
	MemoryPool();
	MemoryPool(const char* poolName, void* original, void* nextalign);
	~MemoryPool();

	// pool functions
	unsigned int getPoolID();
	void* getOriginalMemStart();
	MemoryPool* getNextNode();
	MemoryPool* getPrevNode();
	void SetNextNode(MemoryPool* _nextPool);
	void SetPrevNode(MemoryPool* _prevPool);
	unsigned int GetPoolSize();

	// block functions
	void* AddBlock(size_t size);
	void DeleteBlock();

	//will align 8 bytes in 64 bit system
	//will align 4 bytes in 32 bit system
private:
	void* nextfreeMemoryBlock;
	void* originalMemoryPtr;
	unsigned int hashName;
	unsigned int _poolSize;
	MemoryPool* prevPool;
	MemoryPool* nextPool;
	MemoryBlock* firstBlock;
	MemoryBlock* lastBlock;
};