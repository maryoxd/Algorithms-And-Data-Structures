#pragma once

#include <string>
#include <vector>

#include <libds/amt/implicit_sequence.h>
#include "Typ.h"

struct PopulationData{
	int female;
	int male;
	int population;
};

struct YearPopulationData {
	int year;
	PopulationData data;
};

class UzemnaJednotka
{
private:
	ds::amt::IS<YearPopulationData> data_;
	std::string name_;
	std::string code_;
	Typ type_;


public:
	UzemnaJednotka(const std::string& name, const std::string& code, Typ type, int year = 0, int male = 0, int female = 0);

	std::string getName() const { return name_; }
	std::string getCode() const { return code_; }
	Typ getType() const { return type_; }
	int getPopulation(int year);
	void addNewData(int year, int male, int female);
	void addPopulationData(int year, int male, int female);
	ds::amt::IS<YearPopulationData> getData() const { return data_; }
	void print(int year);
	void printAllYears();
};
