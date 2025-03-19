#pragma once
#include <vector>

class Algoritmus
{
public:
	template<typename T, typename Iterator, typename Predicate>
	std::vector<T> filter(Iterator begin, Iterator end, Predicate predicate);
	
};

template<typename T, typename Iterator, typename Predicate>
std::vector<T> Algoritmus::filter(Iterator begin, Iterator end, Predicate predicate) {
	std::vector<T> filtered_vector;

	for (auto it = begin; it != end; ++it) {
		T object = *it;
		if (predicate(object)) {
			filtered_vector.emplace_back(object);
		}
	}
	return filtered_vector;
}
