#include "Village.h"
#include <sstream>

Village::Village(int year, std::string name, std::string code, int male, int female, int population)
	: year_ (year), name_(name), code_(code), male_(male), female_(female), population_(population) {}

int Village::getYear() const
{
	return year_;
}

std::string Village::getName() const
{
	return name_;
}

std::string Village::getCode() const
{
	return code_;
}

int Village::getMale() const
{
	return male_;
}

int Village::getFemale() const
{
	return female_;
}

int Village::getPopulation() const
{
	return population_;
}

std::string Village::toString() const
{
	std::ostringstream oss;
	oss << "City: " << name_
		<< " | " << code_
		<< " | M: " << male_
		<< " | F: " << female_
		<< " | P: " << population_;
	return oss.str();
}



