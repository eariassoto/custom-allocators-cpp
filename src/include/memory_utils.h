#pragma once
#include <cassert>
#include <cinttypes>

inline uintptr_t AlignAddress(uintptr_t address, size_t align) {
    // For an align n, this creates a mask with 1's in the
    // log2(n) least significant bits. For example, for a
    // align of 16 bits, it creates this mask:
    // 0b00001111
    const size_t mask = align - 1;
    // align can only be a power of two
    assert((align & mask) == 0);
    return (address + mask) & ~mask;
}

template <typename T>
inline T* AlignPointer(T* pointer, size_t align) {
    const uintptr_t address = reinterpret_cast<uintptr_t>(pointer);
    const uintptr_t addressAligned = AlignAddress(address, align);
    return reinterpret_cast<T*>(addressAligned);
}
