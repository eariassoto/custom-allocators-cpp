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
    if (m_AddressList.GetListSize() == 0) {
        return nullptr;
    }
    void* blockPtr = m_AddressList.GetAddress();
    assert(blockPtr != nullptr);

    return blockPtr;
}

void PoolAllocator::FreeBlock(void* blockPtr) {
    if (blockPtr < m_AllocatedMemory || blockPtr > m_LastAllocatedMemory) {
        return;
    }
    size_t offsetFromMemory = reinterpret_cast<size_t>(blockPtr) -
                              reinterpret_cast<size_t>(m_AllocatedMemory);
    if ((offsetFromMemory % m_BlockSizeInBytes) != 0) {
        return;
    }
    m_AddressList.AddAddress(blockPtr);
}

void PoolAllocator::Clear() {
    assert(m_AllocatedMemory != nullptr);

    m_AddressList.Clear();

    uintptr_t currentMemoryBlock =
        reinterpret_cast<uintptr_t>(m_AllocatedMemory);
    for (size_t i = 0; i < m_PoolSize; ++i) {
        void* currBlockPtr = reinterpret_cast<void*>(currentMemoryBlock);
        m_LastAllocatedMemory = currBlockPtr;
        m_AddressList.AddAddress(currBlockPtr);
        currentMemoryBlock += m_BlockSizeInBytes;
    }
}

size_t PoolAllocator::GetAvailableBlocks() const {
    return m_AddressList.GetListSize();
}

