#pragma once

#include "UzemnaJednotka.h"
#include "UzemnaJednotkaTable.h"
#include "Typ.h"

#include <vector>
#include <string>
#include <iostream>

#include <libds/amt/explicit_hierarchy.h>
#include <libds/adt/table.h>

class Loader
{
private:
	using Hierarchy = ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>;
	using HierarchyBlock = ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>;

	std::vector<UzemnaJednotka*> uzemneJednotky_;
	UzemnaJednotkaTable tabulkyUJ_;

public:
	explicit Loader() = default;
	~Loader();

	void loadCsv(const std::string& filename);
	void loadCsv(const std::vector<std::string>& filenames);

	std::vector<UzemnaJednotka*> getVillages() const;

	void loadUzemia(Hierarchy* hierarchy);
	void updateCumulativeData(Hierarchy* hierarchy);
	void updateNodeData(HierarchyBlock* node, Hierarchy* hierarchy);

	UzemnaJednotka* containsUJ(const std::string& name, Typ typ, const std::string& code) const;

	void printAllVillages() const;

	size_t getSize() const;

	void insert(UzemnaJednotka* uj) { tabulkyUJ_.insert(uj); }
	UzemnaJednotkaTable& getTables() { return tabulkyUJ_; }
	void clear();
};
