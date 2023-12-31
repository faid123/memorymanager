#include <stdio.h>
#include <iostream>
#include "GlobalManager.h"

MemoryManager::MemoryManager() : firstPool { nullptr }, lastPool { nullptr }
{
	firstPool = nullptr;
	lastPool = nullptr;
	defaultPoolID = -1;
}

MemoryManager::~MemoryManager()
{
}

//void* operator new(size_t size)
//{
//	printf("new 1 is called\n");
//	return GlobalManager::GetMemoryManager()->AddBlock(size);
//}
//void* operator new(size_t size, const nothrow_t&)
//{
//	DebugPrint("new 2 called");
//	return nullptr;
//}
//
//void* operator new[](size_t size)
//{
//	DebugPrint("new 3 called");
//	return nullptr;
//}
//void* operator new[](size_t size, const nothrow_t&)
//{
//	DebugPrint("new 4 called");
//	return nullptr;
//}
//
//void operator delete(void* ptr)
//{
//
//}
//void operator delete(void* ptr, const nothrow_t&)
//{
//
//}
//void operator delete[](void* ptr)
//{
//
//}
//void operator delete[](void* ptr, const nothrow_t&)
//{
//
//}
//
//void* operator new(size_t, int poolID)
//{
//	return nullptr;
//}

#pragma region public functions handling pool requests
unsigned int MemoryManager::newPool(const int poolSize, const char* poolName)
{
	void* alignedpoolPtr;
	void* alignedmemoryPtr;
	void* originalpoolPtr;
	MemoryPool* newPool;
	//MemoryPool* prevLastPool;

	std::cout << std::endl << std::endl << "***********ADDING NEW MEMORY POOL*********" << std::endl;

	// allocate the memory aligned then use placement new to correction call the constructor
	alignedpoolPtr = AlignMalloc((static_cast<__int64>(poolSize)), MEMORY_ALIGNMENT, &originalpoolPtr);
	if (!alignedpoolPtr)
	{
		return 0;
	}

	// reinterpret_cast<unsigned long long> to compile in a x64 bit environment
	alignedmemoryPtr = reinterpret_cast<void*>(reinterpret_cast<unsigned long long>(alignedpoolPtr) + sizeof(MemoryPool));
	newPool = new(alignedpoolPtr) MemoryPool(poolName, originalpoolPtr, alignedmemoryPtr);
	
	//// update the memory manager and memory pool pointers
	LinkedListAdd(newPool, firstPool, lastPool);
	//prevLastPool = lastPool;
	//// if there is no current pool, assign the first pool to the new allocated address
	//if (!firstPool)
	//{
	//	firstPool = newPool;
	//}
	//// lastpool is the newest allocated memory hence, assignment to newPool always
	//lastPool = newPool;
	//// if no curr pool, prevlastpool = nullptr, otherwise set it to the last pool
	//newPool->SetPrevPool(prevLastPool);
	//// always a nullptr for *next everytime when u allocate new pool
	//newPool->SetNextPool(nullptr);
	//// othercase for next pool, if the previous pool existed
	//if (prevLastPool)
	//{
	//	prevLastPool->SetNextPool(newPool);
	//}
	std::cout << "POOL SIZE: " << poolSize << "KB" << std::endl;
	std::cout << "Mem Pool 1->first: " << firstPool << std::endl;
	std::cout << "Mem Pool 1->last: " << lastPool << std::endl;
	MemoryPool* pool = firstPool;
	unsigned int counter = 0;
	while (pool)
	{
		cout << "Pool " << counter++ << ": " << pool 
				<< ", Prev ptr: " << pool->getPrevNode()
				<< ", Next ptr: " << pool->getNextNode()
				<< endl;
		pool = pool->getNextNode();
	}

	// save the pool id
	return newPool->getPoolID();
}

void MemoryManager::deletePool(const unsigned int poolID)
{
	MemoryPool* deprecatedPool;
	void* memoryStart;

	std::cout << std::endl << std::endl << "***********DELETING MEMORY POOL*********" << std::endl;

	deprecatedPool = firstPool;
	// finding the pool
	while (deprecatedPool)
	{
		if (deprecatedPool->getPoolID() == poolID)
		{
			LinkedListDelete(deprecatedPool, firstPool, lastPool);
			//if (deprecatedPool == firstPool)
			//	firstPool = deprecatedPool->getNextNode();

			//if (deprecatedPool == lastPool)
			//	lastPool = deprecatedPool->getPrevNode();

			//// link pool ptr back if deleting from middle of the pool
			//if (deprecatedPool->getPrevNode())
			//	deprecatedPool->getPrevNode()->SetNextNode(deprecatedPool->getNextNode());

			//// link pool ptr back if deleting from middle of the pool
			//if (deprecatedPool->getNextNode())
			//	deprecatedPool->getNextNode()->SetPrevNode(deprecatedPool->getPrevNode());

			// delete the pool
			memoryStart = deprecatedPool->getOriginalMemStart();
			deprecatedPool->~MemoryPool();
			free(memoryStart);

			std::cout << "Mem Pool 1->first: " << firstPool << std::endl;
			std::cout << "Mem Pool 1->last: " << lastPool << std::endl;
			MemoryPool* pool = firstPool;
			unsigned int counter = 0;
			while (pool)
			{
				std::cout << "Pool " << counter++ << ": " << pool
					<< ", Prev ptr: " << pool->getPrevNode()
					<< ", Next ptr: " << pool->getNextNode()
					<< std::endl;
				pool = pool->getNextNode();
			}

			return;
		}
		else
		{
			deprecatedPool = deprecatedPool->getNextNode();
		}
	}
}

void MemoryManager::deletePool(const char* name)
{
	unsigned int nameHash;
	nameHash = GlobalManager::GetHash()->hash(name, sizeof(name));
	deletePool(nameHash);
}

void MemoryManager::deleteAllPool()
{
	while (firstPool)
	{
		deletePool(firstPool->getPoolID());
	}
}


void MemoryManager::defragPool()
{
}

void MemoryManager::queryPool()
{
}

#pragma endregion

#pragma region public functions handling block requests


void* MemoryManager::AddBlock(size_t size)
{
	return AddBlock(GetDefaultPoolID(), size);
}

void* MemoryManager::AddBlock(int poolID, size_t size)
{
	MemoryPool* pool;

	pool = FindPool(poolID);
	if (!pool)
		return nullptr;

	return pool->AddBlock(size);
}

void* MemoryManager::AssignBlock(size_t type_size)
{
	void* assigned_mem;
	// using assigned_mem to return the blcok of memory in pool that is already allocated in the heap
	assigned_mem = reinterpret_cast<void*>((firstPool)+type_size);
	return assigned_mem;
}

#pragma endregion


void* MemoryManager::AlignMalloc(size_t requiredBytes, const size_t alignment, void** original)
{
	requiredBytes += sizeof(MemoryPool) + (alignment - 1);
	*original = new char[requiredBytes];
	if (!*original)
	{
		//TODO: malloc error, add to log file and wait
		// output debug print message
		return nullptr;
	}

	// alignment-1, alignment is 16 10000
	//							 15 01111
	// or ~ and & will change the address to the aligned
	// reinterpret_case - tell the compiler that i am sure of the type i want
	// since it is a void* 
	return nextalignedMemoryCalculator(*original, alignment);
}

int MemoryManager::GetDefaultPoolID()
{
	if (defaultPoolID == -1)
	{
		if (!firstPool)
			return -1;

		defaultPoolID = firstPool->getPoolID();
	}
	return defaultPoolID;
}

unsigned int MemoryManager::querySpecificPool(const char* poolName)
{
	unsigned int nameHash;
	MemoryPool* poolLookup = nullptr;
	nameHash = GlobalManager::GetHash()->hash(poolName, sizeof(poolName));
	poolLookup = FindPool(nameHash);
	return poolLookup->GetPoolSize();
}

MemoryPool* MemoryManager::FindPool(int poolID)
{
	MemoryPool* foundPool;

	foundPool = firstPool;
	while (foundPool)
	{
		if (foundPool->getPoolID() == poolID)
			return foundPool;

		foundPool = foundPool->getNextNode();
	}

	return nullptr;
}

