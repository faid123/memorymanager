#include "MemoryBlock.h"

MemoryBlock::MemoryBlock(void* data)
{
	dataPtr = data;
	_prevBlock = nullptr;
	_nextBlock = nullptr;
	optimalLoc = false;
}

MemoryBlock::~MemoryBlock()
{
}

void* MemoryBlock::GetDataPtr()
{
	return dataPtr;
}

MemoryBlock* MemoryBlock::GetPrevNode()
{
	return _prevBlock;
}
MemoryBlock* MemoryBlock::GetNextNode()
{
	return _nextBlock;
}

void MemoryBlock::SetPrevNode(MemoryBlock* prevBlock)
{
	_prevBlock = prevBlock;
}
void MemoryBlock::SetNextNode(MemoryBlock* nextBlock)
{
	_nextBlock = nextBlock;
}

