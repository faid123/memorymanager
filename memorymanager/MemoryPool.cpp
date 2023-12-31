#include "MemoryPool.h"
#include "GlobalManager.h"
#include "MemoryBlock.h"

MemoryPool::MemoryPool()
{
	hashName = 101;
	nextfreeMemoryBlock = nullptr;
	originalMemoryPtr = nullptr;
	nextPool = nullptr;
	prevPool = nullptr;
}
MemoryPool::MemoryPool(const char* poolName, void* original, void* nextalign) : nextPool{ nullptr }, prevPool{ nullptr }
{
	originalMemoryPtr = original;
	nextfreeMemoryBlock = nextalign;
	firstBlock = nullptr;
	lastBlock = nullptr;
	hashName = GlobalManager::GetHash()->hash(poolName, sizeof(poolName));
}
MemoryPool::~MemoryPool()
{

}

unsigned int MemoryPool::getPoolID()
{
	return hashName;
}
void* MemoryPool::getOriginalMemStart()
{
	return originalMemoryPtr;
}
MemoryPool* MemoryPool::getNextNode()
{
	return nextPool;
}
MemoryPool* MemoryPool::getPrevNode()
{
	return prevPool;
}
void MemoryPool::SetNextNode(MemoryPool *_nextPool)
{
	nextPool = _nextPool;
}
void MemoryPool::SetPrevNode(MemoryPool* _prevPool)
{
	prevPool = _prevPool;
}
void* MemoryPool::AddBlock(size_t size)
{
	MemoryBlock* newBlock;
	void* data;

	// use placement new to call the constructor of the block class
	data = nextalignedMemoryCalculator(reinterpret_cast<void*>(reinterpret_cast<intptr_t>(nextfreeMemoryBlock)+sizeof(MemoryBlock)), MEMORY_ALIGNMENT);
	newBlock = new(nextfreeMemoryBlock) MemoryBlock(data);

	nextfreeMemoryBlock = nextalignedMemoryCalculator(reinterpret_cast<void*>(reinterpret_cast<intptr_t>(nextfreeMemoryBlock) + sizeof(data) + size), MEMORY_ALIGNMENT);
	
	// update the memory pool and memory block pointers
	LinkedListAdd(newBlock, firstBlock, lastBlock);

	return data;
}
void DeleteBlock()
{
	// update the memory pool and memory block pointers

}

unsigned int MemoryPool::GetPoolSize()
{
	return _poolSize;
}