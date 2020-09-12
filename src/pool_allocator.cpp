#include "pool_allocator.h"

#include <cassert>
#include <cstdlib>

PoolAllocator::PoolAllocator(size_t blockSizeInBytes, size_t poolSize)
    : m_BlockSizeInBytes{blockSizeInBytes}, m_PoolSize{poolSize} {
    // Block size smaller than the size of a pointer is not supported yet
    assert(m_BlockSizeInBytes > sizeof(void*));
    assert(poolSize > 0);

    m_AllocatedMemory = malloc(m_PoolSize * m_BlockSizeInBytes);

    Clear();
}

PoolAllocator::~PoolAllocator() { free(m_AllocatedMemory); }

void* PoolAllocator::AllocateBlock() {
    if (m_AvailableBlocks == 0) {
        return nullptr;
    }
    void* blockPtr = m_FreeBlockListHead;
    assert(blockPtr != nullptr);
    m_FreeBlockListHead =
        reinterpret_cast<List*>(m_FreeBlockListHead->nextFreeBlock);
    m_AvailableBlocks -= 1;
    return blockPtr;
}

void PoolAllocator::FreeBlock(void* blockPtr) {
    if (blockPtr < m_AllocatedMemory || blockPtr > m_LastAllocatedMemory) {
        return;
    }
    size_t offsetFromMemory = reinterpret_cast<size_t>(blockPtr) - reinterpret_cast<size_t>(m_AllocatedMemory);
    if ((offsetFromMemory % m_BlockSizeInBytes) != 0) {
        return;
    }
    AddBlockToList(blockPtr);
}

void PoolAllocator::Clear() {
    assert(m_AllocatedMemory != nullptr);

    m_AvailableBlocks = 0;
    m_FreeBlockListHead = nullptr;

    uintptr_t currentMemoryBlock =
        reinterpret_cast<uintptr_t>(m_AllocatedMemory);
    for (size_t i = 0; i < m_PoolSize; ++i) {
        void* currBlockPtr = reinterpret_cast<void*>(currentMemoryBlock);
        m_LastAllocatedMemory = currBlockPtr;
        AddBlockToList(currBlockPtr);
        currentMemoryBlock += m_BlockSizeInBytes;
    }
}

size_t PoolAllocator::GetAvailableBlocks() const { return m_AvailableBlocks; }

void PoolAllocator::AddBlockToList(void* blockAddress) {
    List* newHead = new (blockAddress) List();
    if (m_FreeBlockListHead == nullptr) {
        m_FreeBlockListHead = newHead;
    } else {
        newHead->nextFreeBlock = m_FreeBlockListHead;
        m_FreeBlockListHead = newHead;
    }
    m_AvailableBlocks += 1;
}
