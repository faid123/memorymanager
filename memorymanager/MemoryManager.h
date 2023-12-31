// headers
#include <iostream>
#include "MemoryPool.h"
#include "MemoryHelper.h"

using namespace std;

class MemoryManager : public MemoryHelper
{
public:
	MemoryManager();
	~MemoryManager();

	// pool functions
	unsigned int newPool(const int poolSize, const char* poolName);
	void deletePool(const unsigned int poolID);
	void deletePool(const char* name);
	void deleteAllPool();
	void defragPool();
	void queryPool();
	unsigned int querySpecificPool(const char* poolName);

	// block functions
	void* AddBlock(size_t size);
	void* AddBlock(int poolID, size_t size);
	void DeleteBlock(void* ptr);
	void DeleteBlock(int poolID, void* ptr);
	void* AssignBlock(size_t type_size);

private:
	int defaultPoolID;
	MemoryPool* firstPool;
	MemoryPool* lastPool;

	void* AlignMalloc(size_t requiredBytes, const size_t alignment, void** original);
	MemoryPool* FindPool(int poolID);
	int GetDefaultPoolID();
};

// global override for new and delete operator
//void* operator new(size_t size);
//void* operator new(size_t size, const nothrow_t&);
//void* operator new[](size_t size);
//void* operator new[](size_t size, const nothrow_t&);
//
//void operator delete(void* ptr);
//void operator delete(void* ptr, const nothrow_t&);
//void operator delete[](void* ptr);
//void operator delete[](void* ptr, const nothrow_t&);
//
//void operator delete(void* ptr, size_t);
//void operator delete(void* ptr, size_t, nothrow_t&);
//
//void* operator new(size_t, int poolID);