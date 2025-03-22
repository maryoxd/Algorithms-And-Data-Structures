#pragma once
#include "Village.h"
#include <vector>
#include <string>
#include <iostream>

class Loader
{
private:
	std::vector<Village> villages_;

public:
	void loadCsv(std::string& filename);
	void loadCsv(std::vector<std::string>& filenames);
	void printVillages();
	void printVillages(int year) const;
	std::vector<Village> getVillages();
	size_t getSize() const;
	size_t getSize(int year) const;
};