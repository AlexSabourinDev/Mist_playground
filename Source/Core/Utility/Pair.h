#pragma once

template< typename First, typename Second >
struct Pair
{
	First m_First;
	Second m_Second;
};

template< typename First, typename Second >
Pair<First, Second> Create(First first, Second second)
{
	return Pair<First, Second>{ first, second };
}
