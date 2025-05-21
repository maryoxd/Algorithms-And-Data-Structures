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

    // Vr�ti v�etky UJ s rovnak�m n�zvom a typom
    bool tryFindAll(const std::string& name, Typ typ, ds::adt::ImplicitList<UzemnaJednotka*>*& result) const;

    // Vr�ti konkr�tnu UJ pod�a n�zvu a k�du
    bool tryFind(const std::string& name, Typ typ, const std::string& code, UzemnaJednotka*& result) const;

    const TabulkaUJ& getTable(Typ typ) const;
    TabulkaUJ& getTable(Typ typ);
    void printTableContent();

    void clear();
};
