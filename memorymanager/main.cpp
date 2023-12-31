// memorymanager.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include  "crtdbg.h"
#include "GlobalManager.h"
#include <vector>
#include <map>

//// put this right after the flag settings from above
//#ifdef _DEBUG
//#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
//
//#endif

using namespace std;

void EnableMemoryLeakChecking(int breakAlloc = -1)
{
	//Set the leak checking flag
	int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpDbgFlag);

	//If a valid break alloc provided set the breakAlloc
	if (breakAlloc != -1) _CrtSetBreakAlloc(breakAlloc);
}

class GOC
{
public:
	GOC() = default;
	~GOC() = delete;
	int GOC_id;
	std::string name;
	std::map<int, char> map_name;

};

int main()
{
	EnableMemoryLeakChecking();
    std::cout << "Memory Management Tool\n";

    int* int_ptr = new int;
    *int_ptr = 4;
    cout << *int_ptr << endl;
	delete int_ptr;
	int_ptr = nullptr;

	GlobalManager::GetMemoryManager()->newPool(500, "four");

	int_ptr = reinterpret_cast<int*>(GlobalManager::GetMemoryManager()->AddBlock(sizeof(int)));
	*int_ptr = 10;
	std::cout << *int_ptr << endl;

	int* ptr = reinterpret_cast<int*>(GlobalManager::GetMemoryManager()->AddBlock(sizeof(int)));
	*ptr = 1;
	cout << *ptr << endl;

	const char* str = "hellow";
	char* char_ptr = reinterpret_cast<char*>(GlobalManager::GetMemoryManager()->AddBlock(sizeof(char)*(strlen(str)+1)));
	strcpy(char_ptr, str);
	cout << char_ptr << endl;

	cout << "sizeof GOC " << sizeof(GOC) << endl;
	GOC* GOC_ptr = reinterpret_cast<GOC*>(GlobalManager::GetMemoryManager()->AddBlock(sizeof(GOC)));
	new (GOC_ptr) GOC();
	GOC_ptr->GOC_id = 123;
	GOC_ptr->name = "testinfas12312409";
	GOC_ptr->map_name.insert(pair<int, char>(1, 'A'));
	GOC_ptr->map_name.insert(pair<int, char>(2, 'B'));
	GOC_ptr->map_name.insert(pair<int, char>(3, 'C'));
	cout << "GOC_id: " << GOC_ptr->GOC_id << endl;
	cout << "GOC_name: " << GOC_ptr->name << endl;

	map<int, char>::iterator itr;
	cout << "\nThe map_name is : \n";
	cout << "\tKEY\tELEMENT\n";
	for (itr = GOC_ptr->map_name.begin(); itr != GOC_ptr->map_name.end(); ++itr) {
		cout << '\t' << itr->first
			<< '\t' << itr->second << '\n';
	}
	cout << endl;

	// remove all elements up to 
	// element with key=3 in gquiz2 
	cout << "\GOC_ptr->map_name after removal of"
		" elements less than key=3 : \n";
	cout << "\tKEY\tELEMENT\n";
	GOC_ptr->map_name.erase(GOC_ptr->map_name.begin(), GOC_ptr->map_name.find(3));
	for (itr = GOC_ptr->map_name.begin(); itr != GOC_ptr->map_name.end(); ++itr) {
		cout << '\t' << itr->first
			<< '\t' << itr->second << '\n';
	}

	GlobalManager::GetMemoryManager()->deleteAllPool();


}
