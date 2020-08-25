#pragma once

class StackAllocator
{
public:
	typedef size_t Marker;

	StackAllocator() = delete;

	explicit StackAllocator(size_t stackSizeInBytes);

	// do not allow copies
	StackAllocator(const StackAllocator& other) = delete;
	StackAllocator& operator = (const StackAllocator& other) = delete;

	StackAllocator(StackAllocator&& other) = default;
	StackAllocator& operator=(StackAllocator && other) = default;

	~StackAllocator();

	void* Allocate(size_t sizeInBytes);

	Marker GetMarker() const;

	void FreeToMarker(Marker marker);

	void Clear();

private:
	
	void* m_AllocatedMemory = 0;
	size_t m_StackSizeInBytes = 0;

	Marker m_CurrentMarker = 0;
};