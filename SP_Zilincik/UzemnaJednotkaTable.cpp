#include "UzemnaJednotkaTable.h"

#include <stdexcept>

void UzemnaJednotkaTable::insert(UzemnaJednotka* uj)
{
    const std::string& key = uj->getName();
    TabulkaUJ& tabulka = getTable(uj->getType());

    ds::adt::ImplicitList<UzemnaJednotka*>** zoznamPtr = nullptr;
    if (!tabulka.tryFind(key, zoznamPtr)) {
        auto* newList = new ds::adt::ImplicitList<UzemnaJednotka*>();
        tabulka.insert(key, newList);
        newList->insertLast(uj);
    }
    else {
        (*zoznamPtr)->insertLast(uj);
    }

}

bool UzemnaJednotkaTable::tryFindAll(const std::string& name, Typ typ, ds::adt::ImplicitList<UzemnaJednotka*>*& result) const
{
    ds::adt::ImplicitList<UzemnaJednotka*>** listPtr = nullptr;
    if (getTable(typ).tryFind(name, listPtr)) {
        result = *listPtr;
        return true;
    }
    return false;
}


bool UzemnaJednotkaTable::tryFind(const std::string& name, Typ typ, const std::string& code, UzemnaJednotka*& result) const
{
    ds::adt::ImplicitList<UzemnaJednotka*>** listPtr = nullptr;
    if (getTable(typ).tryFind(name, listPtr)) {
        for (UzemnaJednotka* uj : **listPtr) {
            if (uj->getCode() == code) {
                result = uj;
                return true;
            }
        }
    }
    return false;
}

const UzemnaJednotkaTable::TabulkaUJ& UzemnaJednotkaTable::getTable(Typ typ) const
{
    switch (typ) {
    case Typ::OBEC: return tabulkaObci_;
    case Typ::REGION: return tabulkaRegionov_;
    case Typ::REPUBLIKA: return tabulkaRepublik_;
    case Typ::GEO: return tabulkaGeo_;
    default: throw std::invalid_argument("Neznámy typ územnej jednotky");
    }
}

UzemnaJednotkaTable::TabulkaUJ& UzemnaJednotkaTable::getTable(Typ typ)
{
    return const_cast<TabulkaUJ&>(
        static_cast<const UzemnaJednotkaTable&>(*this).getTable(typ)
        );
}

void UzemnaJednotkaTable::printTableContent()
{
    auto vypisUJTabulku = [](const std::string& nadpis, TabulkaUJ& tabulka) {
        std::cout << "=== " << nadpis << " ===\n";

        for (auto& pair : tabulka) {
            const std::string& key = pair.key_;
            ds::adt::ImplicitList<UzemnaJednotka*>* list = pair.data_;

            std::cout << "[Kľúč: " << key << "] -> "
                      << (list ? std::to_string(list->size()) : "0")
                      << " jednotiek\n";

            if (list) {
                for (UzemnaJednotka* uj : *list) {
                    std::cout << "   - Názov: " << uj->getName()
                              << ", Kód: " << uj->getCode()
                              << ", Typ: " << static_cast<int>(uj->getType()) << "\n";
                    uj->printAllYears();
                    std::cout << "\n";
                }
            }
        }

        std::cout << "\n";
    };

    vypisUJTabulku("OBCE", tabulkaObci_);
    vypisUJTabulku("REGIONY", tabulkaRegionov_);
    vypisUJTabulku("REPUBLIKY", tabulkaRepublik_);
    vypisUJTabulku("GEO OBLASTI", tabulkaGeo_);
}

void UzemnaJednotkaTable::clear()
{
    auto clearUJTable = [](TabulkaUJ& table) {
        for (auto& [key, list] : table) {
            if (list != nullptr) {
                delete list;
            }
        }
        table.clear();
        };

    clearUJTable(tabulkaObci_);
    clearUJTable(tabulkaRegionov_);
    clearUJTable(tabulkaRepublik_);
    clearUJTable(tabulkaGeo_);
}

