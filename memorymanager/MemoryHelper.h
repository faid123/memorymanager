#pragma once

class MemoryHelper
{
protected:
	MemoryHelper() = default;
	virtual ~MemoryHelper() = default;

	void* nextalignedMemoryCalculator(void* original, size_t alignment);
	template <class T>
	void LinkedListAdd(T& newNode, T& firstNode, T& lastNode)
	{
		T prevLastNode;

		// update the memory manager and memory Node pointers
		prevLastNode = lastNode;
		// if there is no current Node, assign the first Node to the new allocated address
		if (!firstNode)
		{
			firstNode = newNode;
		}
		// lastNode is the newest allocated memory hence, assignment to newNode always
		lastNode = newNode;
		// if no curr Node, prevlastNode = nullptr, otherwise set it to the last Node
		newNode->SetPrevNode(prevLastNode);
		// always a nullptr for *next everytime when u allocate new Node
		newNode->SetNextNode(nullptr);
		// othercase for next Node, if the previous Node existed
		if (prevLastNode)
		{
			prevLastNode->SetNextNode(newNode);
		}
	}

	template <class T>
	void LinkedListDelete(T& deprecatedNode, T& firstNode, T& lastNode)
	{
		if (deprecatedNode == firstNode)
			firstNode = deprecatedNode->getNextNode();

		if (deprecatedNode == lastNode)
			lastNode = deprecatedNode->getPrevNode();

		// link Node ptr back if deleting from middle of the Node
		if (deprecatedNode->getPrevNode())
			deprecatedNode->getPrevNode()->SetNextNode(deprecatedNode->getNextNode());

		// link Node ptr back if deleting from middle of the Node
		if (deprecatedNode->getNextNode())
			deprecatedNode->getNextNode()->SetPrevNode(deprecatedNode->getPrevNode());
	}
};
