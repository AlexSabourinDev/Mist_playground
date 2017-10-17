#pragma once

#include <Mist_Common\include\UtilityMacros.h>

MIST_NAMESPACE

template< typename IteratorType >
struct SearchRange {
	IteratorType* m_Begin = nullptr;
	IteratorType* m_End = nullptr;
};

template< typename CompareType >
using ComparisonFunction = bool(*)(const CompareType* left, const CompareType* right);

template< typename CompareType >
struct Comparator {
	ComparisonFunction<CompareType> m_Compare = nullptr;
	const CompareType m_CompareValue;
};

template< typename ValueType >
ValueType Find(const SearchRange<ValueType*> range, Comparator<ValueType> comparator);


// -Implementation-
template< typename ValueType >
ValueType* Find(const SearchRange<ValueType> range, Comparator<ValueType> comparator) {
	MIST_ASSERT(comparator.m_Compare != nullptr);
	MIST_ASSERT(range.m_Begin != nullptr);

	for (ValueType* iter = range.m_Begin; iter != range.m_End; ++iter) {
		if (comparator.m_Compare(iter, &m_CompareValue)) {
			return iter;
		}
	}
	return nullptr;
}

MIST_NAMESPACE_END