#include <string>
#include <vector>
#include <Windows.h>
#include "Loader.h"
#include "Algoritmus.h"
#include "HierarchyNavigator.h"
#include <libds/amt/explicit_hierarchy.h>

void showMainMenu() {
    std::cout << "--- Hlavné menu ---\n"
        << "1. Úroveò 1 - Filtrovanie obcí\n"
        << "2. Úroveò 2 - Navigácia v hierarchii\n"
        << "3. Ukonèi program\n"
        << "Vaša možnos: ";
}

void showFirstLevelMenu() {
    std::cout << "\n--- Úroveò 1 - Obce ---\n"
        << "1. Vyh¾adávanie pod¾a názvu\n"
        << "2. Poèet obyvate¾ov <= maximálny\n"
        << "3. Poèet obyvate¾ov >= minimálny\n"
        << "4. Vypísa všetky obce\n"
        << "5. Spä\n"
        << "Vaša možnos: ";
}

void showSecondLevelMenu() {
    std::cout << "\n--- Úroveò 2 - Hierarchia ---\n"
        << "1. Ís na rodièa\n"
        << "2. Ís na potomka\n"
        << "3. Zobrazi všetkých potomkov\n"
        << "4. Použi predikát\n"
        << "5. Údaje o populácii aktuálneho vrcholu\n"
        << "6. Spä\n"
        << "Vaša možnos: ";
}

void showPredicateMenu() {
    std::cout << "\n--- Predikáty ---\n"
        << "1. Názov obsahuje reazec\n"
        << "2. Poèet obyvate¾ov <= maximálny\n"
        << "3. Poèet obyvate¾ov >= minimálny\n"
        << "4. Typ územnej jednotky\n"
        << "Vaša možnos: ";
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
    root.data_ = new UzemnaJednotka("RAKÚSKO", "0", Typ::ROOT, 0, 0);

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
                    std::cout << "Zadajte reazec: ";
                    std::string str;
                    std::cin >> str;

                    auto filtered = algo.FilterWithContainsStr(villages, str);

                    std::cout << "Chcete filtrova aj pod¾a roka? (ano/nie): ";
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
                    std::cout << "Zadajte rok a maximálny poèet obyvate¾ov: ";
                    std::cin >> year >> maxResidents;

                    auto filtered = algo.FilterWithHasMaxResidents(villages, year, maxResidents);
                    algo.PrintItems(filtered.begin(), filtered.end(), year);
                    break;
                }
                case 3: { 
                    int year, minResidents;
                    std::cout << "Zadajte rok a minimálny poèet obyvate¾ov: ";
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
                    std::cout << "Neplatná vo¾ba.\n";
                }
            }
            break;
        }

        case 2: { 
            bool secondLevel = true;
            HierarchyNavigator navigator(hierarchy);

            while (secondLevel) {
                auto* current = navigator.getCurrent();
                std::cout << "\nAktuálna pozícia: " << current->data_->getName() << "\n";
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
                        std::cout << "Zadaj reazec: ";
                        std::cin >> str;
                        auto filtered = algo.FilterWithContainsStr(potomkovia, str);
                        algo.PrintItems(filtered.begin(), filtered.end());
                        break;
                    }
                    case 2: {
                        int year, maxResidents;
                        std::cout << "Zadaj rok a maximálny poèet: ";
                        std::cin >> year >> maxResidents;
                        auto filtered = algo.FilterWithHasMaxResidents(potomkovia, year, maxResidents);
                        algo.PrintItems(filtered.begin(), filtered.end(), year);
                        break;
                    }
                    case 3: {
                        int year, minResidents;
                        std::cout << "Zadaj rok a minimálny poèet: ";
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
                        std::cout << "[ERROR] Neplatná vo¾ba predikátu.\n";
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
                                << " | Ženy: " << ypd.data.female
                                << " | Muži: " << ypd.data.male
                                << " | Celkovo: " << ypd.data.population << '\n';
                        }
                    }
                    break;
                }
                case 6:
                    secondLevel = false;
                    break;
                default:
                    std::cout << "Neplatná vo¾ba.\n";
                }
            }
            break;
        }

        case 3:
            running = false;
            break;
        default:
            std::cout << "Neplatná vo¾ba.\n";
            break;
        }
    }
}
