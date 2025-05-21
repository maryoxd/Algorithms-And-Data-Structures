#define NOMINMAX 
#include <string>
#include <vector>

#include <Windows.h>

#include "Loader.h"
#include "Algoritmus.h"
#include "HierarchyNavigator.h"
#include "Table_Analyzer.h"
#include "SortAlgoritmus.h"
#include "Gender.h"

#include <libds/amt/explicit_hierarchy.h>
#include <libds/adt/sorts.h>

void showMainMenu() {
    std::cout << "--- Hlavné menu ---\n"
        << "1. Úroveò 1 - Filtrovanie obcí\n"
        << "2. Úroveò 2 - Navigácia v hierarchii\n"
        << "3. Úroveò 3 - Vyh¾adávanie pod¾a mena a typu\n"
        << "4. Úroveò 3 - Vypísa všetky tabu¾ky\n"
        << "5. Ukonèi program\n"
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
        << "8. Spä\n"
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
    initHeapMonitor();
    SetConsoleOutputCP(1250);
    SetConsoleCP(1250);

    ds::adt::QuickSort<UzemnaJednotka*> quickSort;
    SortAlgoritmus sorter;

    Loader loader;
    std::vector<std::string> filenames = { "2020.csv", "2021.csv", "2022.csv", "2023.csv", "2024.csv" };
    loader.loadCsv(filenames);

    std::vector<UzemnaJednotka*> villages = loader.getVillages();
    Algoritmus algo;

    auto* hierarchy = new ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>();
    auto& root = hierarchy->emplaceRoot();
    root.data_ = new UzemnaJednotka("RAKÚSKO", "0", Typ::ROOT, 0, 0);
    HierarchyNavigator navigator(hierarchy);

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
                    std::cout << "Zadajte reazec: \n";
                    std::string sortAnswer, typeOfSort;
                    std::string str;
                    std::cin >> str;
                    int year;
                    std::cout << "Zadajte rok: \n";
                    std::cin >> year;

                    auto filtered = algo.filterWithContainsStr<UzemnaJednotka*>(villages.begin(), villages.end(), str);
                    
                    std::cout << "Chcete použi SORT?[A/N]\n";
                    std::cin >> sortAnswer;
                    if (sortAnswer == "A") {
                        std::cout << "SORT ABECEDY [A] / SORT POPULÁCIE [M/Ž/P]?\n";
                        std::cin >> typeOfSort;
                        algo.filterSortAndPrint(filtered, sorter, quickSort, year, typeOfSort);
                    }
                    else {
                        algo.printItems(filtered.begin(), filtered.end(), year);
                    }
                    break;
                }
                case 2: { 
                    int year, maxResidents;
                    std::string sortAnswer, typeOfSort;
                    std::cout << "Zadajte rok: \n";
                    std::cin >> year;
                    std::cout << "Zadajte maximálny poèet obyvate¾ov: \n";
                    std::cin >> maxResidents;
                    auto filtered = algo.filterWithHasMaxResidents<UzemnaJednotka*>(villages.begin(), villages.end(), year, maxResidents);
                    
                    std::cout << "Chcete použi SORT?[A/N]\n";
                    std::cin >> sortAnswer;
                    if (sortAnswer == "A") {
                        std::cout << "SORT ABECEDY [A] / SORT POPULÁCIE [M/Ž/P]?\n";
                        std::cin >> typeOfSort;
                        algo.filterSortAndPrint(filtered, sorter, quickSort, year, typeOfSort);
                    }
                    else {
                        algo.printItems(filtered.begin(), filtered.end(), year);
                    }
                    break;

                }
                case 3: { 
                    int year, minResidents;
                    std::string sortAnswer, typeOfSort;
                    std::cout << "Zadajte rok: \n";
                    std::cin >> year;
                    std::cout << "Zadajte minimálny poèet obyvate¾ov: \n";
                    std::cin >> minResidents;
                    auto filtered = algo.filterWithHasMinResidents<UzemnaJednotka*>(villages.begin(), villages.end(), year, minResidents);
  
                    std::cout << "Chcete použi SORT?[A/N]\n";
                    std::cin >> sortAnswer;
                    if (sortAnswer == "A") {
                        std::cout << "SORT ABECEDY [A] / SORT POPULÁCIE [M/Ž/P]?\n";
                        std::cin >> typeOfSort;
                        algo.filterSortAndPrint(filtered, sorter, quickSort, year, typeOfSort);
                    }
                    else {
                        algo.printItems(filtered.begin(), filtered.end(), year);
                    }
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
                    std::cout << "Zadajte index syna: \n";
                    int idx;
                    std::cin >> idx;
                    navigator.moveToChild(idx);
                    break;
                case 3: {
                    auto begin = ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>::PreOrderHierarchyIterator(hierarchy, current);
                    auto end = ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>::PreOrderHierarchyIterator(hierarchy, nullptr);
                    algo.printItems(begin, end);
                    break;
                }
                case 4: {
                    auto begin = ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>::PreOrderHierarchyIterator(hierarchy, current);
                    auto end = ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>::PreOrderHierarchyIterator(hierarchy, nullptr);

                    showPredicateMenu();
                    int predikat;
                    std::cin >> predikat;

                    switch (predikat) {
                    case 1: {
                        std::string str;
                        std::string sortAnswer;
                        int year;
                        std::cout << "Zadajte reazec: \n";
                        std::cin >> str;
                        std::cout << "Zadajte rok: \n";
                        std::cin >> year;
                        auto filtered = algo.filterWithContainsStr<UzemnaJednotka*>(begin, end, str);

                        std::cout << "Chcete použi SORT?[A/N]\n";
                        std::cin >> sortAnswer;
                        if (sortAnswer == "A") {
                            std::string typeOfSort;
                            std::cout << "SORT ABECEDY [A] / SORT POPULÁCIE [M/Ž/P]?\n";
                            std::cin >> typeOfSort;
                            algo.filterSortAndPrint(filtered, sorter, quickSort, year, typeOfSort);
                        }
                        else {
                            algo.printItems(filtered.begin(), filtered.end(), year);
                        }
                        break;
                    }
                    case 2: {
                        int year, maxResidents;
                        std::string sortAnswer;
                        std::cout << "Zadajte rok: \n";
                        std::cin >> year;
                        std::cout << "Zadajte maximálny poèet obyvate¾ov: \n";
                        std::cin >> maxResidents;
                        auto filtered = algo.filterWithHasMaxResidents<UzemnaJednotka*>(begin, end, year, maxResidents);

                        std::cout << "Chcete použi SORT?[A/N]\n";
                        std::cin >> sortAnswer;
                        if (sortAnswer == "A") {
                            std::string typeOfSort;
                            std::cout << "SORT ABECEDY [A] / SORT POPULÁCIE [M/Ž/P]?\n";
                            std::cin >> typeOfSort;
                            algo.filterSortAndPrint(filtered, sorter, quickSort, year, typeOfSort);
                        }
                        else {
                            algo.printItems(filtered.begin(), filtered.end(), year);
                        }
                        break;

                    }
                    case 3: {
                        int year, minResidents;
                        std::string sortAnswer;
                        std::cout << "Zadajte rok: \n";
                        std::cin >> year;
                        std::cout << "Zadajte minimálny poèet obyvate¾ov: \n";
                        std::cin >> minResidents;
                        auto filtered = algo.filterWithHasMinResidents<UzemnaJednotka*>(begin, end, year, minResidents);

                        std::cout << "Chcete použi SORT?[A/N]\n";
                        std::cin >> sortAnswer;
                        if (sortAnswer == "A") {
                            std::string typeOfSort;
                            std::cout << "SORT ABECEDY [A] / SORT POPULÁCIE [M/Ž/P]?\n";
                            std::cin >> typeOfSort;
                            algo.filterSortAndPrint(filtered, sorter, quickSort, year, typeOfSort);
                        }
                        else {
                            algo.printItems(filtered.begin(), filtered.end(), year);
                        }
                        break;

                    }
                    case 4: {
                        int typInput;
                        std::cout << "Zadajte typ (0-ROOT, 1-GEO, 2-REPUBLIKA, 3-REGION, 4-OBEC): \n";
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
                        current->data_->printAllYears();
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
        case 3: {
            std::string name;
            int typInput;

            std::cout << "Zadajte názov jednotky: \n";
            std::cin >> name;
 
            std::cout << "Zadajte typ (0-ROOT, 1-GEO, 2-REPUBLIKA, 3-REGION, 4-OBEC): \n";
            std::cin >> typInput;

            Typ typ = static_cast<Typ>(typInput);

            ds::adt::ImplicitList<UzemnaJednotka*>* zoznam = nullptr;

            if (loader.getTables().tryFindAll(name, typ, zoznam)) {
                std::cout << "[INFO] Nájdených " << zoznam->size() << " jednotiek s názvom '" << name << "':\n";

                for (UzemnaJednotka* uj : *zoznam) {
                    std::cout << "---------------------------------\n";
                    uj->printAllYears();  
                }
            }
            else {
                std::cout << "[INFO] Nenájdené žiadne jednotky s názvom '" << name << "'.\n";
            }
            break;
        }
        case 4:
            loader.getTables().printTableContent();
            break;
        case 5:
            running = false;
            break;
        default:
            std::cout << "Neplatná vo¾ba.\n";
            break;
        }
    }

    navigator.clearHierarchy();
    delete hierarchy;
    return 0;
}
