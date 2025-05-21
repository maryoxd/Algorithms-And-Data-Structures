#pragma once

#include <vector>
#include "Predicates.h"
#include "SortAlgoritmus.h"
#include <string>
#include <libds/adt/sorts.h>


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

    template<typename T>
    void filterSortAndPrint(
        const std::vector<T>& filtered,
        SortAlgoritmus& sorter,
        ds::adt::QuickSort<T>& quickSort,
        int year,
        const std::string& typeOfSort
    );

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

template<typename T>
void Algoritmus::filterSortAndPrint(
    const std::vector<T>& filtered,
    SortAlgoritmus& sorter,
    ds::adt::QuickSort<T>& quickSort,
    int year,
    const std::string& typeOfSort
) {
    ds::amt::ImplicitSequence<T> pomocnaSekvencia;

    for (int i = 0; i < filtered.size(); ++i) {
        pomocnaSekvencia.insertLast().data_ = filtered[i];
    }

        if (typeOfSort == "A") {
            quickSort.sort(pomocnaSekvencia, sorter.compareAlphabetical());
        }
        else if (typeOfSort == "M" || typeOfSort == "m") {
            sorter.setPopulationSortCriteria(year, Gender::MALE);
            quickSort.sort(pomocnaSekvencia, sorter.comparePopulation());
        }
        else if (typeOfSort == "Z" || typeOfSort == "z") {
            sorter.setPopulationSortCriteria(year, Gender::FEMALE);
            quickSort.sort(pomocnaSekvencia, sorter.comparePopulation());
        }
        else if (typeOfSort == "P" || typeOfSort == "p") {
            sorter.setPopulationSortCriteria(year, Gender::TOTAL);
            quickSort.sort(pomocnaSekvencia, sorter.comparePopulation());
        }

    for (auto it = pomocnaSekvencia.begin(); it != pomocnaSekvencia.end(); ++it) {
            (*it)->print(year);
    }
}

