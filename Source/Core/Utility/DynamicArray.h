#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <cstdint>
#include <cstdlib>

MIST_NAMESPACE

// -Config-
static const size_t DYNAMIC_ARRAY_GROWTH_SIZE = 5;


// -Data-
template< typename ElementType >
struct DynamicArray {
	ElementType* m_Data = nullptr;
	size_t m_Size = 0;
	size_t m_Capacity = 0;
};


// -API-
template< typename ElementType >
void Reserve(DynamicArray<ElementType>* target, size_t size);

template< typename ElementType >
void PushBack(DynamicArray<ElementType>* target, ElementType element);

template< typename ElementType >
ElementType PopBack(DynamicArray<ElementType>* target);

template< typename ElementType >
void Clear(DynamicArray<ElementType>* target);


// -Implementation-
template< typename ElementType >
void Reserve(DynamicArray<ElementType>* target, size_t size)
{
	if (target->m_Capacity >= size || size == 0)
	{
		return;
	}

	ElementType* result = (ElementType*)realloc(target->m_Data, size * sizeof(ElementType));

	if (result == nullptr)
	{
		// Realloc failed to reallocate the block of memory
		MIST_ASSERT(false);
	}
	else
	{
		target->m_Data = result;
		target->m_Capacity = size;
	}
}

template< typename ElementType >
void PushBack(DynamicArray<ElementType>* target, ElementType element)
{
	if (target->m_Size == target->m_Capacity)
	{
		Reserve(target, target->m_Size + DYNAMIC_ARRAY_GROWTH_SIZE);
	}

	target->m_Data[target->m_Size] = element;
	target->m_Size++;
}

template< typename ElementType >
ElementType PopBack(DynamicArray<ElementType>* target)
{
	MIST_ASSERT(target->m_Size > 0);

	ElementType element = target->m_Data[target->m_Size - 1];
	m_Size--;
	return element;
}

template< typename ElementType >
void Clear(DynamicArray<ElementType>* target)
{
	free(target->m_Data);
	target->m_Size = 0;
	target->m_Capacity = 0;
	target->m_Data = nullptr;
}

MIST_NAMESPACE_END
