#pragma once


class MemoryBlock
{
public:
	MemoryBlock() = delete;
	MemoryBlock(void* data);
	~MemoryBlock();

	void* GetDataPtr();
	MemoryBlock* GetPrevNode();
	MemoryBlock* GetNextNode();
	void SetPrevNode(MemoryBlock* prevBlock);
	void SetNextNode(MemoryBlock* nextBlock);

private:
	MemoryBlock* _prevBlock;
	MemoryBlock* _nextBlock;
	bool optimalLoc;
	void* dataPtr;
};