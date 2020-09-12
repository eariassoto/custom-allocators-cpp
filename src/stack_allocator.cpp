#include "stack_allocator.h"

#include <cassert>
#include <cstdlib>

StackAllocator::StackAllocator(size_t stackSizeInBytes)
    : m_StackSizeInBytes{stackSizeInBytes}, m_CurrentMarker{0} {
    m_AllocatedMemory = malloc(m_StackSizeInBytes);
}

StackAllocator::~StackAllocator() { free(m_AllocatedMemory); }

void* StackAllocator::Allocate(size_t sizeInBytes) {
    assert(m_AllocatedMemory != 0);
    const size_t availableSize = m_StackSizeInBytes - m_CurrentMarker;
    if (sizeInBytes > 0 && availableSize >= sizeInBytes) {
        Marker newMarker = m_CurrentMarker + sizeInBytes;
        uintptr_t assignedAddress =
            reinterpret_cast<uintptr_t>(m_AllocatedMemory) + newMarker;
        m_CurrentMarker = newMarker;
        return reinterpret_cast<void*>(assignedAddress);
    }
    return nullptr;
}

StackAllocator::Marker StackAllocator::GetMarker() const {
    return m_CurrentMarker;
}

void StackAllocator::FreeToMarker(Marker marker) {
    // TODO: markers are not checked. I.e. a random marker
    // can be passed, thus corrupting memory
    if (marker < m_CurrentMarker) {
        m_CurrentMarker = marker;
    }
}

void StackAllocator::Clear() { m_CurrentMarker = 0; }
