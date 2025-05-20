#include "UzemnaJednotkaTable.h"

void UzemnaJednotkaTable::insert(UzemnaJednotka* uj)
{
    const std::string& key = uj->getName();

    try {
        getTable(uj->getType()).insert(key, uj);
    }
    catch (...) {
    }
}

bool UzemnaJednotkaTable::tryFind(const std::string& name, Typ typ, UzemnaJednotka*& result) const
{
    UzemnaJednotka** ptr = nullptr;
    if (getTable(typ).tryFind(name, ptr) && ptr != nullptr) {
        result = *ptr;
        return true;
    }
    return false;
}

const ds::adt::SortedSequenceTable<std::string, UzemnaJednotka*>& UzemnaJednotkaTable::getTable(Typ typ) const
{
    switch (typ) {
    case Typ::OBEC: return tabulkaObci_;
    case Typ::REGION: return tabulkaRegionov_;
    case Typ::REPUBLIKA: return tabulkaRepublik_;
    case Typ::GEO: return tabulkaGeo_;
    default: throw std::invalid_argument("Neznámy typ územnej jednotky");
    }
}

void UzemnaJednotkaTable::clear()
{
    auto clearUJTable = [](auto& table) {
        table.clear();
        };

    clearUJTable(tabulkaObci_);
    clearUJTable(tabulkaRegionov_);
    clearUJTable(tabulkaRepublik_);
    clearUJTable(tabulkaGeo_);
}

ds::adt::SortedSequenceTable<std::string, UzemnaJednotka*>& UzemnaJednotkaTable::getTable(Typ typ)
{
    return const_cast<ds::adt::SortedSequenceTable<std::string, UzemnaJednotka*>&>(
        static_cast<const UzemnaJednotkaTable&>(*this).getTable(typ)
        );
}
