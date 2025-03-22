#pragma once

#include <string>
#define NOT_FOUND std::string::npos

template<typename T>
auto makeContainsStr(const std::string& str) {
	return [str](const T& item) {
		return item.getName().find(str) != NOT_FOUND;
		};
}

template<typename T>
auto makeContainsStr(const std::string& str, int year) {
	return [str, year](const T& item) {
		return item.getName().find(str) != NOT_FOUND && item.getYear() == year;
		};
}

template<typename T>
auto makeMaxResidents(int year, int maxResidents) {
	return [year, maxResidents](const T& item) {
		return item.getYear() == year && item.getPopulation() <= maxResidents;
		};
}

template<typename T>
auto makeMinResidents(int year, int minResidents) {
	return [year, minResidents](const T& item) {
		return item.getYear() == year && item.getPopulation() >= minResidents;
		};
}