#include "MemoryHelper.h"
#include <stdio.h>
#include "MemoryPool.h"

void* MemoryHelper::nextalignedMemoryCalculator(void* original, size_t alignment)
{
	void* aligned;

	printf("Original Location: %p\n", original);

	aligned = reinterpret_cast<void*>((reinterpret_cast<size_t>(original) & ~(alignment - 1)) + alignment);
	// original is 1 byte past whatever you have stored
	if (aligned < original)
	{
		aligned = reinterpret_cast<void*>((reinterpret_cast<size_t>(aligned)) + alignment);
	}

	printf("Aligned Location: %p\n", aligned);

	return aligned;
}
