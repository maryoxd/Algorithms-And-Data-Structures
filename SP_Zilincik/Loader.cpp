#include "Loader.h"
#include <fstream>
#include <sstream>
#include <regex>

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
			
			int female;
			int male;
			male = std::stoi(maleString);
			female = std::stoi(femaleString);

			UzemnaJednotka* najdena = containsUJ(code);
			if (najdena) {
				najdena->addNewData(year, male, female);
			}
			else {
				UzemnaJednotka* nova = new UzemnaJednotka(name, code, Typ::OBEC, year, male, female);
				uzemneJednotky_.emplace_back(nova);
				index_.emplace_back(code, nova);
			}
		}
	}
}

void Loader::loadUzemia(ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>* hierarchy)
{
	auto* root = hierarchy->accessRoot();

	std::ifstream file("uzemie.csv");
	if (!file.is_open()) {
		std::cerr << "ERROR_WHILE_OPENING_FILE\n";
		return;
	}

	std::string line;
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string name, code;

		if (std::getline(ss, name, ';') && std::getline(ss, code, ';')) {
			code = code.substr(3, code.size() - 4);

			std::vector<int> indices;
			for (char c : code) {
				int idx = (c - '0');
				indices.push_back(idx == -1 ? 0 : idx);
			}

			if (indices.size() == 1) {
				hierarchy->emplaceSon(*root, indices[0] - 1).data_ = new UzemnaJednotka(name, code, Typ::GEO);
			}
			else if (indices.size() == 2) {
				auto* geo = hierarchy->accessSon(*root, indices[0] - 1);
				hierarchy->emplaceSon(*geo, indices[1] - 1).data_ = new UzemnaJednotka(name, code, Typ::REPUBLIKA);
			}
			else if (indices.size() == 3) {
				auto* geo = hierarchy->accessSon(*root, indices[0] - 1);
				auto* rep = hierarchy->accessSon(*geo, indices[1] - 1);
				hierarchy->emplaceSon(*rep, (indices[2] == 0 ? 0 : indices[2] - 1)).data_ = new UzemnaJednotka(name, code, Typ::REGION);
			}
		}
	}
	file.close();

	std::ifstream file2("obce.csv");
	if (!file2.is_open()) {
		std::cerr << "ERROR_WHILE_OPENING_FILE\n";
		return;
	}

	while (std::getline(file2, line)) {
		std::stringstream ss(line);
		std::string name, parentName, code;

		if (std::getline(ss, name, ';') && std::getline(ss, parentName, ';') && std::getline(ss, code, ';')) {
			code = code.substr(2);

			std::vector<int> indices;
			for (char c : code.substr(0, 3)) {
				int idx = (c - '0') - 1;
				indices.push_back(idx < 0 ? 0 : idx);
			}

			auto* geo = hierarchy->accessSon(*root, indices[0]);
			auto* rep = hierarchy->accessSon(*geo, indices[1]);
			auto* reg = hierarchy->accessSon(*rep, indices[2]);

			UzemnaJednotka* obec = containsUJ(parentName);
			if (obec && obec->getType() == Typ::OBEC) {
				auto& inserted = hierarchy->emplaceSon(*reg, reg->sons_->size());
				inserted.data_ = obec;
			}
			else {
				std::cerr << "[ERROR] Obec s kodom " << code << " neexistuje.\n";
			}
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

UzemnaJednotka* Loader::containsUJ(const std::string& code) const
{
	for (const auto& entry : index_) {
		if (entry.first == code) {
			return entry.second;
		}
	}
	return nullptr;
}

void Loader::updateCumulativeData(ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>* hierarchy)
{
	auto* root = hierarchy->accessRoot();
	if (root != nullptr) {
		updateNodeData(root, hierarchy);
	}
}

void Loader::updateNodeData(ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>* node, ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>* hierarchy)
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

void Loader::printAllVillages() {
	for (auto v : uzemneJednotky_) {
		v->printAllYears();
	}
}

size_t Loader::getSize() const
{
	return uzemneJednotky_.size();
}

std::vector<UzemnaJednotka*> Loader::getVillages() {
	return uzemneJednotky_;
}


