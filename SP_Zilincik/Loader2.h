#pragma once
#include <string>
#include <unordered_map>
#include <memory>          

#include "UzemnaJednotka.h"
#include "TypVrchola.h"

class Loader2
{
private:
    std::unordered_map<std::string, UzemnaJednotka*> mapRaw_;

public:
    void loadYearCsv(const std::string& filename);
    void Loader2::loadUzemie(const std::string& filename);
    void Loader2::loadObcePrepojenie(const std::string& filename);

    ~Loader2();

    const std::unordered_map<std::string, UzemnaJednotka*>& obce() const
    {
        return mapRaw_;
    }

};
