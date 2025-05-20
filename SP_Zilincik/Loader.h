#pragma once
#include "UzemnaJednotka.h"
#include "UzemnaJednotkaTable.h"
#include <vector>
#include <string>
#include <iostream>
#include "Typ.h"
#include <libds/amt/explicit_hierarchy.h>
#include <libds/adt/table.h>

class Loader
{
private:
	std::vector<UzemnaJednotka*> uzemneJednotky_;
	std::vector<std::pair<std::string, UzemnaJednotka*>> index_;
	UzemnaJednotkaTable tabulkyUJ_;

public:
	Loader() {};
	~Loader();
	void loadCsv(std::string& filename);
	void loadCsv(std::vector<std::string>& filenames);

	std::vector<UzemnaJednotka*> getVillages();
	void loadUzemia(ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>* hierarchy);
	UzemnaJednotka* containsUJ(const std::string& code) const;
	void updateCumulativeData(ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>* hierarchy);
	void updateNodeData(ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>* node, ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>* hierarchy);
	void printAllVillages();
	size_t getSize() const;

	void insert(UzemnaJednotka* uj) { tabulkyUJ_.insert(uj); }
	UzemnaJednotkaTable& getTables() { return tabulkyUJ_; }

	void clear();
};