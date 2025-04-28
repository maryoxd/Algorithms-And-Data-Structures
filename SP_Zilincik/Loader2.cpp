#include "Loader2.h"
#include "TypVrchola.h"
#include <fstream>
#include <sstream>
#include <iostream>


void Loader2::loadYearCsv(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "ERROR_WHILE_OPENING_FILE " << filename << '\n';
        return;
    }
    int year = 0;
    std::string line;

    if (std::getline(file, line))           
    {
        year = std::stoi(line);
    }


    std::getline(file, line);               


    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string name, code, maleStr, femaleStr;

        if (std::getline(ss, name, ';') &&
            std::getline(ss, code, ';') &&
            std::getline(ss, maleStr, ';') &&
            std::getline(ss, femaleStr, ';'))
        {
            int male = std::stoi(maleStr);
            int female = std::stoi(femaleStr);

            UzemnaJednotka*& unit = mapRaw_[code];
            if (unit == nullptr)
            {
                unit = new UzemnaJednotka(name,           
                    code,            
                    TypVrchola::OBEC
                );
            }
            unit->addStats({ year, male, female });
        }
    }
}

void Loader2::loadUzemie(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Chyba pri otváraní súboru uzemie.csv\n";
        return;
    }

    std::string line;
    std::getline(file, line); // preskoč hlavičku

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string kod, nazov, nadrKod;

        std::getline(ss, kod, ';');
        std::getline(ss, nazov, ';');
        std::getline(ss, nadrKod, ';');

        // Trim \r
        if (!kod.empty() && kod.back() == '\r') kod.pop_back();
        if (!nazov.empty() && nazov.back() == '\r') nazov.pop_back();
        if (!nadrKod.empty() && nadrKod.back() == '\r') nadrKod.pop_back();

        // Ak sa jednotka ešte nenachádza, vytvor ju
        UzemnaJednotka*& jednotka = mapRaw_[kod];
        if (jednotka == nullptr)
        {
            jednotka = new UzemnaJednotka(nazov, kod, TypVrchola::REPUBLIKA); // predvolene REPUBLIKA
        }

        // Ak má nadriadený kód, zabezpeč aby aj nadradená existovala
        if (!nadrKod.empty())
        {
            UzemnaJednotka*& nadr = mapRaw_[nadrKod];
            if (nadr == nullptr)
            {
                nadr = new UzemnaJednotka("?", nadrKod, TypVrchola::GEO); // môžeme upraviť neskôr
            }

            // Naviazme vzťah nadradený → syn
            nadr->addChild(jednotka);
        }
    }
}

void Loader2::loadObcePrepojenie(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Chyba pri otváraní súboru obce.csv\n";
        return;
    }

    std::string line;
    std::getline(file, line); // preskoč hlavičku

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string kodObce, nazovObce, kodRegionu;

        std::getline(ss, kodObce, ';');
        std::getline(ss, nazovObce, ';');
        std::getline(ss, kodRegionu, ';');

        // Trim \r
        if (!kodObce.empty() && kodObce.back() == '\r') kodObce.pop_back();
        if (!kodRegionu.empty() && kodRegionu.back() == '\r') kodRegionu.pop_back();

        // Získaj obec
        auto obecIt = mapRaw_.find(kodObce);
        if (obecIt == mapRaw_.end()) {
            std::cerr << "Obec " << kodObce << " nenájdená v načítaných dátach!\n";
            continue;
        }
        UzemnaJednotka* obec = obecIt->second;

        // Získaj región
        UzemnaJednotka*& region = mapRaw_[kodRegionu];
        if (region == nullptr) {
            region = new UzemnaJednotka("?", kodRegionu, TypVrchola::REGION); // vytvoríme ak neexistuje
        }

        // Pridaj obec ako dieťa regiónu
        region->addChild(obec);
    }



Loader2::~Loader2()
{
    for (auto& [_, ptr] : mapRaw_)
        delete ptr;
}
