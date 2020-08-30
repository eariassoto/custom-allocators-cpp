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
    size_t m_BlockSizeInBytes = 0;
    size_t m_PoolSize = 0;
    size_t m_AvailableBlocks = 0;
    struct List {
        void* nextFreeBlock = nullptr;
    };
    List* m_FreeBlockListHead = nullptr;

    void AddBlockToList(void* blockAddress);

    void* m_AllocatedMemory = 0;
    void* m_LastAllocatedMemory = 0;
};