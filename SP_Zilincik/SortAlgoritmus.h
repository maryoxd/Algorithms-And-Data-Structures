#pragma once

#include <string>
#include <functional>
#include <algorithm>

#include "UzemnaJednotka.h"
#include "Gender.h"

class SortAlgoritmus
{
private:
    static constexpr int INVALID_CHAR = 1000;

    const char alphabetOrder_[120] = {
        'A', 'Á', 'Ä', 'B', 'C', 'Č', 'D', 'Ď', 'E', 'É', 'F', 'G',
        'H', 'I', 'Í', 'J', 'K', 'L', 'Ľ', 'M', 'N', 'Ň', 'O', 'Ó', 'Ô', 'Ö',
        'P', 'Q', 'R', 'Ŕ', 'S', 'Š', 'T', 'Ť', 'U', 'Ú', 'Ü', 'V', 'W', 'X', 'Y', 'Ý', 'Z', 'Ž',
        'a', 'á', 'ä', 'b', 'c', 'č', 'd', 'ď', 'e', 'é', 'f', 'g',
        'h', 'i', 'í', 'j', 'k', 'l', 'ľ', 'm', 'n', 'ň', 'o', 'ó', 'ô', 'ö',
        'p', 'q', 'r', 'ŕ', 's', 'š', 'ß', 't', 'ť', 'u', 'ú', 'ü', 'v', 'w', 'x', 'y', 'ý', 'z', 'ž'
    };

    int getCharOrder(char ch) const {
        for (int i = 0; i < sizeof(alphabetOrder_) / sizeof(char); ++i) {
            if (alphabetOrder_[i] == ch)
                return i;
        }
        return INVALID_CHAR;
    }

    std::function<bool(UzemnaJednotka* const&, UzemnaJednotka* const&)> compareAlphabetical_;
    std::function<bool(UzemnaJednotka* const&, UzemnaJednotka* const&)> comparePopulation_;

    int selectedYear_ = 2020;
    Gender selectedGender_ = Gender::TOTAL;

public:
    SortAlgoritmus()
    {
        compareAlphabetical_ = [this](UzemnaJednotka* const& a, UzemnaJednotka* const& b) {
            const std::string& name1 = a->getName();
            const std::string& name2 = b->getName();

            size_t len = std::min(name1.length(), name2.length());
            for (size_t i = 0; i < len; ++i) {
                int ordA = getCharOrder(name1[i]);
                int ordB = getCharOrder(name2[i]);
                if (ordA != ordB)
                    return ordA < ordB;
            }
            return name1.length() < name2.length();
            };

        comparePopulation_ = [this](UzemnaJednotka* const& a, UzemnaJednotka* const& b) {
            int valA = a->getPopulation(selectedYear_, selectedGender_);
            int valB = b->getPopulation(selectedYear_, selectedGender_);
            return valA < valB;
            };
    }

    void setPopulationSortCriteria(int year, Gender gender) {
        selectedYear_ = year;
        selectedGender_ = gender;
    }

    std::function<bool(UzemnaJednotka* const&, UzemnaJednotka* const&)> compareAlphabetical() const {
        return compareAlphabetical_;
    }

    std::function<bool(UzemnaJednotka* const&, UzemnaJednotka* const&)> comparePopulation() const {
        return comparePopulation_;
    }
};
