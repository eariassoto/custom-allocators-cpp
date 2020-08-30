#include <cassert>
#include <iostream>

#include "PoolAllocator.h"
#include "StackAllocator.h"

class MyClass {
    float myFloat;
    int myInt;
    char myCharArr[3];
};

int main() {
    StackAllocator stackAllocator{256};
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

    struct Person {
        std::string name;
        unsigned int age;
    };
    const size_t BLOCK_SIZE_IN_BYTES = sizeof(Person);
    const size_t POOL_SIZE = 100;
    PoolAllocator allocator{BLOCK_SIZE_IN_BYTES, POOL_SIZE};

    Person* p1 = new (allocator.AllocateBlock()) Person();
    p1->age = 42;
    p1->name = "John Smith";

    p1->~Person();
    allocator.FreeBlock(reinterpret_cast<void*>(p1));
}
