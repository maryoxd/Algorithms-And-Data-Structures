#pragma once

#include <vector>
#include "Predicates.h"

class Algoritmus
{
public:
    template<typename T, typename Iterator, typename Predicate>
    std::vector<T> filter(Iterator begin, Iterator end, Predicate predicate);

    template<typename T, typename Iterator>
    std::vector<T> filterWithContainsStr(Iterator begin, Iterator end, const std::string& str);

    template<typename T, typename Iterator>
    std::vector<T> filterWithHasMaxResidents(Iterator begin, Iterator end, int year, int maxResidents);

    template<typename T, typename Iterator>
    std::vector<T> filterWithHasMinResidents(Iterator begin, Iterator end, int year, int minResidents);

    template <typename Iterator>
    void printItems(Iterator begin, Iterator end);

    template <typename Iterator>
    void printItems(Iterator begin, Iterator end, int year);
};

template<typename T, typename Iterator, typename Predicate>
std::vector<T> Algoritmus::filter(Iterator begin, Iterator end, Predicate predicate)
{
    std::vector<T> result;
    for (auto it = begin; it != end; ++it) {
        if (predicate(*it)) {
            result.emplace_back(*it);
        }
    }
    return result;
}

template<typename T, typename Iterator>
std::vector<T> Algoritmus::filterWithContainsStr(Iterator begin, Iterator end, const std::string& str)
{
    return filter<T>(begin, end, makeContainsStr<T>(str));
}

template<typename T, typename Iterator>
std::vector<T> Algoritmus::filterWithHasMaxResidents(Iterator begin, Iterator end, int year, int maxResidents)
{
    return filter<T>(begin, end, makeMaxResidents<T>(year, maxResidents));
}

template<typename T, typename Iterator>
std::vector<T> Algoritmus::filterWithHasMinResidents(Iterator begin, Iterator end, int year, int minResidents)
{
    return filter<T>(begin, end, makeMinResidents<T>(year, minResidents));
}

template <typename Iterator>
void Algoritmus::printItems(Iterator begin, Iterator end)
{
    for (auto it = begin; it != end; ++it) {
        if (*it != nullptr) {
            (*it)->printAllYears();
        }
    }
}

template <typename Iterator>
void Algoritmus::printItems(Iterator begin, Iterator end, int year)
{
    for (auto it = begin; it != end; ++it) {
        if (*it != nullptr) {
            (*it)->print(year);
        }
    }
}
