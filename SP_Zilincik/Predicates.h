#pragma once

#include <string>
#include "Typ.h"

template<typename T>
auto makeContainsStr(const std::string& str) {
	return [str](const T& item) {
		return item->getName().find(str) != std::string::npos;
		};
}

template<typename T>
auto makeMaxResidents(int year, int maxResidents) {
	return [year, maxResidents](const T& item) {
		return item->getPopulation(year) <= maxResidents;
		};
}

template<typename T>
auto makeMinResidents(int year, int minResidents) {
	return [year, minResidents](const T& item) {
		return item->getPopulation(year) >= minResidents;
		};
}

template<typename T>
auto makeHasType(Typ typ) {
	return [typ](const T& item) {
		return item->getType() == typ;
		};
}
