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

		if (std::getline(ss, name, ';') &&
			std::getline(ss, code, ';') &&
			std::getline(ss, maleString, ';') &&
			std::getline(ss, femaleString, ';'))
		{
			int population;
			int female;
			int male;
			male = std::stoi(maleString);
			female = std::stoi(femaleString);
			population = (male + female);
			villages_.emplace_back(year, name, code, male, female, population);

		}
	}
}

void Loader::loadCsv(std::vector<std::string>& filenames)
{
	for (auto& filename : filenames)
	{
		loadCsv(filename);
	}
}

std::string Loader::toString()
{
	std::ostringstream oss;
	for (size_t i = 0; i < villages_.size(); ++i)
	{
		oss << i + 1 << ". " << villages_[i].toString() << "\n";
	}
	return oss.str();
}

std::string Loader::toString(int year) const
{
	std::ostringstream oss;
	oss << "Cities in " << year << ":\n";
	int k = 0;
	for (const auto& village : villages_)
	{
		if (village.getYear() == year)
		{
			oss << ++k << ". " << village.toString() << "\n";
		}
	}
	return oss.str();
}

std::vector<Village> Loader::getVillages() {
	return villages_;
}

size_t Loader::getSize() const
{
	return villages_.size(); 
}

size_t Loader::getSize(int year) const
{
	int k = 0;
	for (const auto& village : villages_)
	{
		if (village.getYear() == year)
		{
			k++;
		}
	}
	return k;
}
