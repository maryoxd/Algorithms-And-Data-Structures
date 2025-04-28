#include <string>
#include <vector>
#include <Windows.h>
#include "Loader.h"
#include "Algoritmus.h"
#include "HierarchyNavigator.h"
#include <libds/amt/explicit_hierarchy.h>

void showMainMenu() {
    std::cout << "--- Hlavn� menu ---\n"
        << "1. �rove� 1 - Filtrovanie obc�\n"
        << "2. �rove� 2 - Navig�cia v hierarchii\n"
        << "3. Ukon�i� program\n"
        << "Va�a mo�nos�: ";
}

void showFirstLevelMenu() {
    std::cout << "\n--- �rove� 1 - Obce ---\n"
        << "1. Vyh�ad�vanie pod�a n�zvu\n"
        << "2. Po�et obyvate�ov <= maxim�lny\n"
        << "3. Po�et obyvate�ov >= minim�lny\n"
        << "4. Vyp�sa� v�etky obce\n"
        << "5. Sp�\n"
        << "Va�a mo�nos�: ";
}

void showSecondLevelMenu() {
    std::cout << "\n--- �rove� 2 - Hierarchia ---\n"
        << "1. �s� na rodi�a\n"
        << "2. �s� na potomka\n"
        << "3. Zobrazi� v�etk�ch potomkov\n"
        << "4. Pou�i� predik�t\n"
        << "5. �daje o popul�cii aktu�lneho vrcholu\n"
        << "6. Sp�\n"
        << "Va�a mo�nos�: ";
}

void showPredicateMenu() {
    std::cout << "\n--- Predik�ty ---\n"
        << "1. N�zov obsahuje re�azec\n"
        << "2. Po�et obyvate�ov <= maxim�lny\n"
        << "3. Po�et obyvate�ov >= minim�lny\n"
        << "4. Typ �zemnej jednotky\n"
        << "Va�a mo�nos�: ";
}

int main() {
    SetConsoleOutputCP(1250);
    SetConsoleCP(1250);

    Loader loader;
    std::vector<std::string> filenames = { "2020.csv", "2021.csv", "2022.csv", "2023.csv", "2024.csv" };
    loader.loadCsv(filenames);

    std::vector<UzemnaJednotka*> villages = loader.getVillages();
    Algoritmus algo;

    auto* hierarchy = new ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>();
    auto& root = hierarchy->emplaceRoot();
    root.data_ = new UzemnaJednotka("RAK�SKO", "0", Typ::ROOT, 0, 0);

    loader.loadUzemia(hierarchy);
    loader.updateCumulativeData(hierarchy);

    bool running = true;
    while (running) {
        showMainMenu();
        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            bool firstLevel = true;
            while (firstLevel) {
                showFirstLevelMenu();
                int option;
                std::cin >> option;

                switch (option) {
                case 1: { 
                    std::cout << "Zadajte re�azec: ";
                    std::string str;
                    std::cin >> str;

                    auto filtered = algo.FilterWithContainsStr(villages, str);

                    std::cout << "Chcete filtrova� aj pod�a roka? (ano/nie): ";
                    std::string answer;
                    std::cin >> answer;

                    if (answer == "ano") {
                        int year;
                        std::cout << "Zadajte rok: ";
                        std::cin >> year;
                        algo.PrintItems(filtered.begin(), filtered.end(), year);
                    }
                    else {
                        algo.PrintItems(filtered.begin(), filtered.end());
                    }
                    break;
                }
                case 2: { 
                    int year, maxResidents;
                    std::cout << "Zadajte rok a maxim�lny po�et obyvate�ov: ";
                    std::cin >> year >> maxResidents;

                    auto filtered = algo.FilterWithHasMaxResidents(villages, year, maxResidents);
                    algo.PrintItems(filtered.begin(), filtered.end(), year);
                    break;
                }
                case 3: { 
                    int year, minResidents;
                    std::cout << "Zadajte rok a minim�lny po�et obyvate�ov: ";
                    std::cin >> year >> minResidents;

                    auto filtered = algo.FilterWithHasMinResidents(villages, year, minResidents);
                    algo.PrintItems(filtered.begin(), filtered.end(), year);
                    break;
                }
                case 4:
                    loader.printAllVillages();
                    break;
                case 5:
                    firstLevel = false;
                    break;
                default:
                    std::cout << "Neplatn� vo�ba.\n";
                }
            }
            break;
        }

        case 2: { 
            bool secondLevel = true;
            HierarchyNavigator navigator(hierarchy);

            while (secondLevel) {
                auto* current = navigator.getCurrent();
                std::cout << "\nAktu�lna poz�cia: " << current->data_->getName() << "\n";
                showSecondLevelMenu();
                int option;
                std::cin >> option;

                switch (option) {
                case 1:
                    navigator.moveToParent();
                    break;
                case 2:
                    navigator.listChildren();
                    std::cout << "Zadajte index syna: ";
                    int idx;
                    std::cin >> idx;
                    navigator.moveToChild(idx);
                    break;
                case 3: {
                    auto begin = ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>::PreOrderHierarchyIterator(hierarchy, current);
                    auto end = ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>::PreOrderHierarchyIterator(hierarchy, nullptr);
                    algo.PrintItems(begin, end);
                    break;
                }
                case 4: {
                    std::vector<UzemnaJednotka*> potomkovia;
                    auto begin = ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>::PreOrderHierarchyIterator(hierarchy, current);
                    auto end = ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>::PreOrderHierarchyIterator(hierarchy, nullptr);

                    for (; begin != end; ++begin) {
                        potomkovia.push_back(*begin);
                    }

                    showPredicateMenu();
                    int predikat;
                    std::cin >> predikat;

                    switch (predikat) {
                    case 1: {
                        std::string str;
                        std::cout << "Zadaj re�azec: ";
                        std::cin >> str;
                        auto filtered = algo.FilterWithContainsStr(potomkovia, str);
                        algo.PrintItems(filtered.begin(), filtered.end());
                        break;
                    }
                    case 2: {
                        int year, maxResidents;
                        std::cout << "Zadaj rok a maxim�lny po�et: ";
                        std::cin >> year >> maxResidents;
                        auto filtered = algo.FilterWithHasMaxResidents(potomkovia, year, maxResidents);
                        algo.PrintItems(filtered.begin(), filtered.end(), year);
                        break;
                    }
                    case 3: {
                        int year, minResidents;
                        std::cout << "Zadaj rok a minim�lny po�et: ";
                        std::cin >> year >> minResidents;
                        auto filtered = algo.FilterWithHasMinResidents(potomkovia, year, minResidents);
                        algo.PrintItems(filtered.begin(), filtered.end(), year);
                        break;
                    }
                    case 4: {
                        int typInput;
                        std::cout << "Zadaj typ (0-ROOT, 1-GEO, 2-REPUBLIKA, 3-REGION, 4-OBEC): ";
                        std::cin >> typInput;
                        Typ typ = static_cast<Typ>(typInput);
                        auto pred = makeHasType<UzemnaJednotka*>(typ);

                        if (pred(current->data_))
                            std::cout << "[INFO] Typ sa zhoduje.\n";
                        else
                            std::cout << "[INFO] Typ sa nezhoduje.\n";
                        break;
                    }
                    default:
                        std::cout << "[ERROR] Neplatn� vo�ba predik�tu.\n";
                    }
                    break;
                }
                case 5: {
                    if (current && current->data_) {
                        for (const auto& ypd : current->data_->getData()) {
                            if (ypd.year == 0 && ypd.data.female == 0 && ypd.data.male == 0 && ypd.data.population == 0) {
                                continue;
                            }

                            std::cout << "Rok " << ypd.year
                                << " | �eny: " << ypd.data.female
                                << " | Mu�i: " << ypd.data.male
                                << " | Celkovo: " << ypd.data.population << '\n';
                        }
                    }
                    break;
                }
                case 6:
                    secondLevel = false;
                    break;
                default:
                    std::cout << "Neplatn� vo�ba.\n";
                }
            }
            break;
        }

        case 3:
            running = false;
            break;
        default:
            std::cout << "Neplatn� vo�ba.\n";
            break;
        }
    }
}
