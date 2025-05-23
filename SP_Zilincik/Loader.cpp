#include "Loader.h"

#include <fstream>
#include <sstream>

using Hierarchy = ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>;
using HierarchyBlock = ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>;

Loader::~Loader()
{
	clear();
}

void Loader::loadCsv(const std::string& filename)
{
	std::ifstream file(filename);

	if (!file.is_open())
	{
		std::cerr << "[ERROR] Failed to open CSV file " << filename << ".\n";
		return;
	}

	int year;
	std::string line;

	if (!std::getline(file, line)) {
		std::cerr << "[ERROR] CSV doesn't have a valid year.\n";
		return;
	}

	try {
		year = std::stoi(line);
	}
	catch (...) {
		std::cerr << "[ERROR] CSV doesn't have a valid year.\n";
		return;
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

			int female;
			int male;
			male = std::stoi(maleString);
			female = std::stoi(femaleString);

			UzemnaJednotka* najdena = containsUJ(name, Typ::OBEC, code);

			if (najdena) {
				najdena->addNewData(year, male, female);

			}
			else {
				UzemnaJednotka* nova = new UzemnaJednotka(name, code, Typ::OBEC, year, male, female);
				uzemneJednotky_.emplace_back(nova);
				insert(nova);
			}
		}
	}
}

void Loader::loadUzemia(Hierarchy* hierarchy)
{
	auto* root = hierarchy->accessRoot();

	std::ifstream file("uzemie.csv");
	if (!file.is_open()) {
		std::cerr << "[ERROR] Failed to open CSV file uzemie.csv.\n";
		return;
	}

	std::string line;
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string name, code;

		if (std::getline(ss, name, ';') && std::getline(ss, code, ';')) {
			code = code.substr(3, code.size() - 4);

			size_t len = code.length();
			if (len == 0 || len > 3) {
				std::cerr << "[WARN] Skipping invalid code: " << code << ".\n";
				continue;
			}

			int geoIdx = (len >= 1) ? code[0] - '0' : 0;
			int repIdx = (len >= 2) ? code[1] - '0' : 0;
			int regIdx = (len == 3) ? code[2] - '0' : 0;

			if (geoIdx > 0) geoIdx--;
			if (repIdx > 0) repIdx--;
			if (regIdx > 0) regIdx--;

			if (len == 1) {
				auto* nova = new UzemnaJednotka(name, code, Typ::GEO);
				hierarchy->emplaceSon(*root, geoIdx).data_ = nova;
				insert(nova);
			}
			else if (len == 2) {
				auto* geo = hierarchy->accessSon(*root, geoIdx);
				auto* nova = new UzemnaJednotka(name, code, Typ::REPUBLIKA);
				hierarchy->emplaceSon(*geo, repIdx).data_ = nova;
				insert(nova);
			}
			else if (len == 3) {
				auto* geo = hierarchy->accessSon(*root, geoIdx);
				auto* rep = hierarchy->accessSon(*geo, repIdx);
				auto* nova = new UzemnaJednotka(name, code, Typ::REGION);
				hierarchy->emplaceSon(*rep, regIdx).data_ = nova;
				insert(nova);
			}
		}
	}
	file.close();

	std::ifstream file2("obce.csv");
	if (!file2.is_open()) {
		std::cerr << "[ERROR] Failed to open CSV file obce.csv.\n";
		return;
	}

	while (std::getline(file2, line)) {
		std::stringstream ss(line);
		std::string name, parentName, code;

		if (std::getline(ss, name, ';') &&
			std::getline(ss, parentName, ';') &&
			std::getline(ss, code, ';'))
		{
			code = code.substr(2);
			if (code.length() < 3) {
				std::cerr << "[WARN] Skipping invalid code: " << code << ".\n";
				continue;
			}

			int geoIdx = code[0] - '0' - 1;
			int repIdx = code[1] - '0' - 1;
			int regIdx = code[2] - '0' - 1;

			geoIdx = (geoIdx < 0 ? 0 : geoIdx);
			repIdx = (repIdx < 0 ? 0 : repIdx);
			regIdx = (regIdx < 0 ? 0 : regIdx);

			auto* geo = hierarchy->accessSon(*root, geoIdx);
			auto* rep = hierarchy->accessSon(*geo, repIdx);
			auto* reg = hierarchy->accessSon(*rep, regIdx);

			UzemnaJednotka* obec = containsUJ(name, Typ::OBEC, parentName);
			if (obec && obec->getType() == Typ::OBEC) {
				auto& inserted = hierarchy->emplaceSon(*reg, reg->sons_->size());
				inserted.data_ = obec;
			}
			else {
				std::cerr << "[ERROR] VILLAGE WITH CODE " << code << " doesn't exists.\n";
			}
		}
	}
}

void Loader::loadCsv(const std::vector<std::string>& filenames)
{
	for (auto& filename : filenames)
	{
		loadCsv(filename);
	}
}

UzemnaJednotka* Loader::containsUJ(const std::string& name, Typ typ, const std::string& code) const
{
	UzemnaJednotka* result = nullptr;
	if (tabulkyUJ_.tryFind(name, typ, code, result)) {
		return result;
	}
	return nullptr;
}

void Loader::updateCumulativeData(Hierarchy* hierarchy)
{
	auto* root = hierarchy->accessRoot();
	if (root != nullptr) {
		updateNodeData(root, hierarchy);
	}
}

void Loader::updateNodeData(HierarchyBlock* node, Hierarchy* hierarchy)
{
	if (node == nullptr || node->data_ == nullptr) return;

	size_t sonsCount = hierarchy->degree(*node);
	for (size_t i = 0; i < sonsCount; ++i) {
		auto* son = hierarchy->accessSon(*node, i);
		if (son != nullptr) {
			updateNodeData(son, hierarchy);
		}
	}

	for (size_t i = 0; i < sonsCount; ++i) {
		auto* son = hierarchy->accessSon(*node, i);
		if (son != nullptr && son->data_ != nullptr) {
			for (const auto& [year, pdata] : son->data_->getData()) {
				node->data_->addPopulationData(year, pdata.male, pdata.female);
			}
		}
	}
}

void Loader::printAllVillages() const {
	for (auto item : uzemneJednotky_) {
		item->printAllYears();
	}
}

size_t Loader::getSize() const
{
	return uzemneJednotky_.size();
}

void Loader::clear()
{
	for (UzemnaJednotka* item : uzemneJednotky_) {
		delete item;
	}

	uzemneJednotky_.clear();
	tabulkyUJ_.clear();
}

std::vector<UzemnaJednotka*> Loader::getVillages() const {
	return uzemneJednotky_;
}