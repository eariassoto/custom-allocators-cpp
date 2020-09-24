#include "pool_allocator.h"

#include <cassert>
#include <cstdlib>

PoolAllocator::PoolAllocator(size_t blockSizeInBytes, size_t poolSize)
    : m_RequestedBlockSizeInBytes{blockSizeInBytes}, m_PoolSize{poolSize} {
    // Block size smaller than the size of a pointer is not supported yet
    assert(m_RequestedBlockSizeInBytes > sizeof(void*));
    assert(poolSize > 0);

    // Add one more byte at the end to mark allocated blocks and avoid
    // multiple deallocation of the same block
    m_ReadBlockSizeInBytes = m_RequestedBlockSizeInBytes + 1;

    
    m_AllocatedMemory = malloc(m_PoolSize * m_ReadBlockSizeInBytes);

    Clear();
}

PoolAllocator::~PoolAllocator() { free(m_AllocatedMemory); }

void* PoolAllocator::AllocateBlock() {
    if (m_AddressList.GetListSize() == 0) {
        return nullptr;
    }
    void* blockPtr = m_AddressList.GetAddress();
    assert(blockPtr != nullptr);

    char* blockFlag =
        reinterpret_cast<char*>(blockPtr) + m_RequestedBlockSizeInBytes;
    *blockFlag = FLAG_BLOCK_ALLOCATED;

    return blockPtr;
}

void PoolAllocator::FreeBlock(void* blockPtr) {
    // Check that the address is within range
    if (blockPtr < m_AllocatedMemory || blockPtr > m_LastAllocatedMemory) {
        return;
    }

    // Check that address is an actual block offset
    size_t offsetFromMemory = reinterpret_cast<size_t>(blockPtr) -
                              reinterpret_cast<size_t>(m_AllocatedMemory);
    if ((offsetFromMemory % m_ReadBlockSizeInBytes) != 0) {
        return;
    }

    // Find the last byte at the end of the requested block size to mark it as
    // allocated
    char* blockFlag =
        reinterpret_cast<char*>(blockPtr) + m_RequestedBlockSizeInBytes;
    if (*blockFlag == FLAG_BLOCK_ALLOCATED) {
        m_AddressList.AddAddress(blockPtr);
        *blockFlag = FLAG_BLOCK_FREE;
    }  
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
        currentMemoryBlock += m_ReadBlockSizeInBytes;
    }
}

size_t PoolAllocator::GetAvailableBlocks() const {
    return m_AddressList.GetListSize();
}

