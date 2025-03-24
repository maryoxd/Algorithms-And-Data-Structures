#pragma once
#include <vector>
#include "Predicates.h"

class Algoritmus
{
public:
	template<typename T, typename Iterator, typename Predicate>
	std::vector<T> filter(Iterator begin, Iterator end, Predicate predicate);

	template<typename T>
	std::vector<T> FilterWithContainsStr(const std::vector<T>& object, std::string str);

	template<typename T>
	std::vector<T> FilterWithContainsStr(const std::vector<T>& object, std::string str, int year);

	template<typename T>
	std::vector<T> FilterWithHasMaxResidents(const std::vector<T>& object, int year, int maxResidents);

	template<typename T>
	std::vector<T> FilterWithHasMinResidents(const std::vector<T>& object, int year, int minResidents);

	template <typename Iterator>
	void PrintItems(Iterator begin, Iterator end);
	
};

template<typename T, typename Iterator, typename Predicate>
std::vector<T> Algoritmus::filter(Iterator begin, Iterator end, Predicate predicate) {
	std::vector<T> result;

	for (auto it = begin; it != end; ++it) {
		T object = *it;
		if (predicate(object)) {
			result.emplace_back(object);
		}
	}
	return result;
}

template<typename T>
std::vector<T> Algoritmus::FilterWithContainsStr(const std::vector<T>& object, std::string str)
{
	return this->filter<T>(object.begin(), object.end(), makeContainsStr<T>(str));
}

template<typename T>
std::vector<T> Algoritmus::FilterWithContainsStr(const std::vector<T>& object, std::string str, int year)
{
	return this->filter<T>(object.begin(), object.end(), makeContainsStr<T>(str, year));
}

template<typename T>
std::vector<T> Algoritmus::FilterWithHasMaxResidents(const std::vector<T>& object, int year, int maxResidents)
{
	return this->filter<T>(object.begin(), object.end(), makeMaxResidents<T>(year, maxResidents));
}

template<typename T>
std::vector<T> Algoritmus::FilterWithHasMinResidents(const std::vector<T>& object, int year, int minResidents)
{
	return this->filter<T>(object.begin(), object.end(), makeMinResidents<T>(year, minResidents));
}

template <typename Iterator>
void Algoritmus::PrintItems(Iterator begin, Iterator end)
{
	for (auto it = begin; it != end; ++it)
	{
		std::cout << it->toString() << '\n';
	}
}
