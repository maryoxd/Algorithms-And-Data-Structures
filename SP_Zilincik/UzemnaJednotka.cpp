#include "UzemnaJednotka.h"

#include <algorithm>
#include <sstream>
#include <iostream>

UzemnaJednotka::UzemnaJednotka(const std::string& name, const std::string& code, Typ type, int year, int male, int female)
	: name_(name), code_(code), type_(type)
{
	auto& block = data_.insertLast();
	block.data_.year = year;
	block.data_.data.female = female;
	block.data_.data.male = male;
	block.data_.data.population = male + female;
}

int UzemnaJednotka::getPopulation(int year) 
{
	for (auto it = data_.begin(); it != data_.end(); ++it) {
		const YearPopulationData& record = *it;  
		if (record.year == year) {
			return record.data.population;
		}
	}
	return 0; 
}

int UzemnaJednotka::getPopulation(int year, Gender gender)
{
	for (auto it = data_.begin(); it != data_.end(); ++it) {
		const YearPopulationData& record = *it;
		if (record.year == year) {
			switch (gender) {
			case Gender::MALE:   return record.data.male;
			case Gender::FEMALE: return record.data.female;
			case Gender::TOTAL:  return record.data.population;
			}
		}
	}
	return 0;
}

void UzemnaJednotka::addNewData(int year, int male, int female)
{
	auto& block = data_.insertLast();  
	block.data_.year = year;            
	block.data_.data.female = female;   
	block.data_.data.male = male;       
	block.data_.data.population = male + female; 
}

void UzemnaJednotka::addPopulationData(int year, int male, int female)
{
	for (auto it = data_.begin(); it != data_.end(); ++it) {
		if ((*it).year == year) {
			(*it).data.male += male;
			(*it).data.female += female;
			(*it).data.population += male + female;
			return;
		}
	}

	auto& block = data_.insertLast();
	block.data_ = { year, { female, male, male + female } };
}

void UzemnaJednotka::print(int year)
{
	for (auto it = data_.begin(); it != data_.end(); ++it) {
		const YearPopulationData& record = *it;
		if (record.year == year) {
			const PopulationData& pdata = record.data;
			std::cout << name_
				<< " | R: " << year
				<< " | C: " << code_
				<< " | M: " << pdata.male
				<< " | Ž: " << pdata.female
				<< " | P: " << pdata.population
				<< " | T: " << typToString(type_)
				<< "\n";
			return;
		}
	}
}

void UzemnaJednotka::printAllYears()
{
	std::cout << name_ << " C: " << code_ << "\n";
	for (auto it = data_.begin(); it != data_.end(); ++it) {
		const YearPopulationData& record = *it;

		if (record.year == 0 && record.data.female == 0 && record.data.male == 0 && record.data.population == 0) {
			continue;
		}

		int year = record.year;
		const PopulationData& pdata = record.data;
		std::cout << "Rok: " << year
			<< " | C: " << code_
			<< " | Ž: " << pdata.female
			<< " | M: " << pdata.male
			<< " | P: " << pdata.population
			<<" | T: " << typToString(type_)
			<< "\n";
	}
}
