#pragma once
#include <string>
#include "UzemnaJednotka.h"
#include <libds/adt/table.h>
#include "Typ.h"

class UzemnaJednotkaTable {
private:
    ds::adt::SortedSequenceTable<std::string, UzemnaJednotka*> tabulkaObci_;
    ds::adt::SortedSequenceTable<std::string, UzemnaJednotka*> tabulkaRegionov_;
    ds::adt::SortedSequenceTable<std::string, UzemnaJednotka*> tabulkaRepublik_;
    ds::adt::SortedSequenceTable<std::string, UzemnaJednotka*> tabulkaGeo_;

public:
    void insert(UzemnaJednotka* uj);
    bool tryFind(const std::string& name, Typ typ, UzemnaJednotka*& result) const;

    const ds::adt::SortedSequenceTable<std::string, UzemnaJednotka*>& getTable(Typ typ) const;
    ds::adt::SortedSequenceTable<std::string, UzemnaJednotka*>& getTable(Typ typ);
    void clear();
};

