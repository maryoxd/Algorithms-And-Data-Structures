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
	void printCities(int year);
	std::vector<City> getCities();
	int getSize();
	int getSize(int year);
};