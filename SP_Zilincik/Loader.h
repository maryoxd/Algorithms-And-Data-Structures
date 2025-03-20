#pragma once
#include "City.h"
#include <vector>
#include <string>
#include <iostream>

class Loader
{
private:
	std::vector<City> cities_;

public:
	void loadCsv(std::string& filename);
	void loadCsv(std::vector<std::string>& filenames);
	void printCities();
	void printCities(int year) const;
	std::vector<City> getCities();
	size_t getSize() const;
	int getSize(int year) const;
};