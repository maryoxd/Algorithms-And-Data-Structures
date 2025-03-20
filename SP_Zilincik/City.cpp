#include "City.h"

City::City(int year, std::string name, std::string code, int male, int female, int population)
	: year_ (year), name_(name), code_(code), male_(male), female_(female), population_(population) {}

int City::getYear() const
{
	return year_;
}

std::string City::getName() const
{
	return name_;
}

std::string City::getCode() const
{
	return code_;
}

int City::getMale() const
{
	return male_;
}

int City::getFemale() const
{
	return female_;
}

int City::getPopulation() const
{
	return population_;
}

void City::print() const
{
	std::cout << "City: " << name_ << " | " << code_ << " | M: " << male_ << " | F: " << female_ << " | " << " P: " << population_ << " | " << '\n';
		
}


