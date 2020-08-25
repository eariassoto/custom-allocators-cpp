#include <iostream>
#include <cassert>
#include "StackAllocator.h"

class MyClass
{
	float myFloat;
	int myInt;
	char myCharArr[3];
};

int main()
{
	StackAllocator stackAllocator{ 256 };
	assert(stackAllocator.GetMarker() == 0);

	void* firstChunk = stackAllocator.Allocate(64);
	StackAllocator::Marker firstMarker = stackAllocator.GetMarker();

	assert(firstChunk != nullptr);
	assert(firstMarker == 64);

	void* secondChunk = stackAllocator.Allocate(64);
	StackAllocator::Marker secondMarker = stackAllocator.GetMarker();

	assert(secondChunk != nullptr);
	assert(secondMarker == 128);

	void* thirdChunk = stackAllocator.Allocate(256);
	StackAllocator::Marker thirdMarker = stackAllocator.GetMarker();

	assert(thirdChunk == nullptr);
	assert(thirdMarker == 128);

	void* fourthChunk = stackAllocator.Allocate(128);
	StackAllocator::Marker fourthMarker = stackAllocator.GetMarker();

	assert(fourthChunk != nullptr);
	assert(fourthMarker == 256);

	stackAllocator.FreeToMarker(secondMarker);
	StackAllocator::Marker markerAfterFree = stackAllocator.GetMarker();
	assert(markerAfterFree == secondMarker);

	stackAllocator.Clear();
	StackAllocator::Marker markerAfterClear = stackAllocator.GetMarker();
	assert(markerAfterClear == 0);

	std::cout << "Hello World!\n";


}
