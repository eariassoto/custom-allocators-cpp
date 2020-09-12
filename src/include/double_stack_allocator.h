#pragma once

class DoubleStackAllocator
{
public:
	typedef size_t Marker;

	DoubleStackAllocator() = delete;

	explicit DoubleStackAllocator(size_t stackSizeInBytes);

	// do not allow copies
	DoubleStackAllocator(const DoubleStackAllocator& other) = delete;
	DoubleStackAllocator& operator = (const DoubleStackAllocator& other) = delete;

	DoubleStackAllocator(DoubleStackAllocator&& other) = default;
	DoubleStackAllocator& operator=(DoubleStackAllocator && other) = default;

	~DoubleStackAllocator();

	void* AllocateLower(size_t sizeInBytes);
	void* AllocateUpper(size_t sizeInBytes);

	Marker GetLowerMarker() const;
	Marker GetUpperMarker() const;

	void FreeToLowerMarker(Marker marker);
	void FreeToUpperMarker(Marker marker);

	void ClearLower();
	void ClearUpper();
	void ClearAll();

private:

	void* m_AllocatedMemory = 0;
	size_t m_StackSizeInBytes = 0;

	Marker m_LowerMarker = 0;
	Marker m_UpperMarker = 0;
};
