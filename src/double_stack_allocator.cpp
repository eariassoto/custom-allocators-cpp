#include "double_stack_allocator.h"

#include <cassert>
#include <cstdlib>

DoubleStackAllocator::DoubleStackAllocator(size_t stackSizeInBytes)
    : m_StackSizeInBytes{stackSizeInBytes},
      m_LowerMarker{0},
      m_UpperMarker{stackSizeInBytes} {
    m_AllocatedMemory = malloc(m_StackSizeInBytes);
}

DoubleStackAllocator::~DoubleStackAllocator() { free(m_AllocatedMemory); }

void* DoubleStackAllocator::AllocateLower(size_t sizeInBytes) {
    assert(m_AllocatedMemory != 0);
    assert(m_UpperMarker > m_LowerMarker);
    const size_t availableSize = m_UpperMarker - m_LowerMarker;
    if (sizeInBytes > 0 && availableSize >= sizeInBytes) {
        Marker newMarker = m_LowerMarker + sizeInBytes;
        uintptr_t assignedAddress =
            reinterpret_cast<uintptr_t>(m_AllocatedMemory) + newMarker;
        m_LowerMarker = newMarker;
        return reinterpret_cast<void*>(assignedAddress);
    }
    return nullptr;
}

void* DoubleStackAllocator::AllocateUpper(size_t sizeInBytes) {
    assert(m_AllocatedMemory != 0);
    assert(m_UpperMarker > m_LowerMarker);
    const size_t availableSize = m_UpperMarker - m_LowerMarker;
    if (sizeInBytes > 0 && availableSize >= sizeInBytes) {
        Marker newMarker = m_UpperMarker - sizeInBytes;
        uintptr_t assignedAddress =
            reinterpret_cast<uintptr_t>(m_AllocatedMemory) + newMarker;
        m_UpperMarker = newMarker;
        return reinterpret_cast<void*>(assignedAddress);
    }
    return nullptr;
}

DoubleStackAllocator::Marker DoubleStackAllocator::GetLowerMarker() const {
    return m_LowerMarker;
}

DoubleStackAllocator::Marker DoubleStackAllocator::GetUpperMarker() const {
    return m_UpperMarker;
}

void DoubleStackAllocator::FreeToLowerMarker(Marker marker) {
    // TODO: markers are not checked. I.e. a random marker
    // can be passed, thus corrupting memory
    if (marker < m_LowerMarker) {
        m_LowerMarker = marker;
    }
}

void DoubleStackAllocator::FreeToUpperMarker(Marker marker) {
    // TODO: markers are not checked. I.e. a random marker
    // can be passed, thus corrupting memory
    if (marker > m_UpperMarker && marker < m_StackSizeInBytes) {
        m_UpperMarker = marker;
    }
}

void DoubleStackAllocator::ClearLower() { m_LowerMarker = 0; }

void DoubleStackAllocator::ClearUpper() { m_UpperMarker = m_StackSizeInBytes; }

void DoubleStackAllocator::ClearAll() {
    ClearLower();
    ClearUpper();
}
