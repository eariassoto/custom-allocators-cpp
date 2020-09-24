#include "address_list.h"
#pragma once

class PoolAllocator {
   public:

    PoolAllocator() = delete;

    explicit PoolAllocator(size_t blockSizeInBytes, size_t poolSize);

    // do not allow copies
    PoolAllocator(const PoolAllocator& other) = delete;
    PoolAllocator& operator=(const PoolAllocator& other) = delete;

    PoolAllocator(PoolAllocator&& other) = default;
    PoolAllocator& operator=(PoolAllocator&& other) = default;

    ~PoolAllocator();

    void* AllocateBlock();

    void FreeBlock(void* blockPtr);

    void Clear();

    size_t GetAvailableBlocks() const;

   private:
    static constexpr char FLAG_BLOCK_ALLOCATED = 0x01;
    static constexpr char FLAG_BLOCK_FREE = 0x00;
    size_t m_RequestedBlockSizeInBytes = 0;
    size_t m_ReadBlockSizeInBytes = 0;
    size_t m_PoolSize = 0;

    AdressList m_AddressList;

    void* m_AllocatedMemory = 0;
    void* m_LastAllocatedMemory = 0;
};