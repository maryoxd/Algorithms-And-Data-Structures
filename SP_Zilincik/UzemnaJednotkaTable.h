#pragma once

#include <string>

#include "UzemnaJednotka.h"
#include "Typ.h"

#include <libds/adt/table.h>
#include <libds/adt/list.h>

class UzemnaJednotkaTable
{
private:
    using TabulkaUJ = ds::adt::SortedSequenceTable<std::string, ds::adt::ImplicitList<UzemnaJednotka*>*>;

    TabulkaUJ tabulkaObci_;
    TabulkaUJ tabulkaRegionov_;
    TabulkaUJ tabulkaRepublik_;
    TabulkaUJ tabulkaGeo_;

public:
    void insert(UzemnaJednotka* uj);

    // Vráti všetky UJ s rovnakým názvom a typom
    bool tryFindAll(const std::string& name, Typ typ, ds::adt::ImplicitList<UzemnaJednotka*>*& result) const;

    // Vráti konkrétnu UJ pod¾a názvu a kódu
    bool tryFind(const std::string& name, Typ typ, const std::string& code, UzemnaJednotka*& result) const;

    const TabulkaUJ& getTable(Typ typ) const;
    TabulkaUJ& getTable(Typ typ);
    void printTableContent();

    void clear();
};
