#include "Loader.h"
#include <fstream>
#include <sstream>

void Loader::loadCsv(std::string& filename)
{
	std::ifstream file(filename);

	if (!file.is_open())
	{
		std::cerr << "ERROR_WHILE_OPENING_FILE" << filename << "\n";
		return;
	}

	int year;

	std::string line;

	if (std::getline(file, line))
	{
		year = std::stoi(line);
	}

	std::getline(file, line);

	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string name, code, maleString, femaleString;
		int male, female;

		if (std::getline(ss, name, ';') &&
			std::getline(ss, code, ';') &&
			std::getline(ss, maleString, ';') &&
			std::getline(ss, femaleString, ';'))
		{
			male = std::stoi(maleString);
			female = std::stoi(femaleString);
			cities_.emplace_back(year, name, code, male, female);

		}
	}
}

void Loader::loadCsv(std::vector<std::string>& filenames)
{
	for (int i = 0; i < filenames.size(); i++)
	{
		loadCsv(filenames[i]);
	}
}

void Loader::printCities()
{
	for (int i = 0; i < cities_.size(); i++)
	{
		std::cout << i+1 << ". ";
		cities_[i].print();
	}
}

void Loader::printCities(int year)
{
	std::cout << "Cities in " << year << ":\n";
	int k = 0;
	for (int i = 0; i < cities_.size(); i++)
	{
		if (cities_[i].getYear() == year)
		{
			k++;
			std::cout << k << ". ";
			cities_[i].print();
		}
	}
}
