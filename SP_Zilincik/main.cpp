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
    std::cout << "--- Hlavn� menu ---\n"
        << "1. �rove� 1 - Filtrovanie obc�\n"
        << "2. �rove� 2 - Navig�cia v hierarchii\n"
        << "3. �rove� 3 - Vyh�ad�vanie pod�a mena a typu\n"
        << "4. �rove� 3 - Vyp�sa� v�etky tabu�ky\n"
        << "5. Ukon�i� program\n"
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
        << "8. Sp�\n"
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
    root.data_ = new UzemnaJednotka("RAK�SKO", "0", Typ::ROOT, 0, 0);
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
                    std::cout << "Zadajte re�azec: \n";
                    std::string sortAnswer, typeOfSort;
                    std::string str;
                    std::cin >> str;
                    int year;
                    std::cout << "Zadajte rok: \n";
                    std::cin >> year;

                    auto filtered = algo.filterWithContainsStr<UzemnaJednotka*>(villages.begin(), villages.end(), str);
                    
                    std::cout << "Chcete pou�i� SORT?[A/N]\n";
                    std::cin >> sortAnswer;
                    if (sortAnswer == "A") {
                        std::cout << "SORT ABECEDY [A] / SORT POPUL�CIE [M/�/P]?\n";
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
                    std::cout << "Zadajte maxim�lny po�et obyvate�ov: \n";
                    std::cin >> maxResidents;
                    auto filtered = algo.filterWithHasMaxResidents<UzemnaJednotka*>(villages.begin(), villages.end(), year, maxResidents);
                    
                    std::cout << "Chcete pou�i� SORT?[A/N]\n";
                    std::cin >> sortAnswer;
                    if (sortAnswer == "A") {
                        std::cout << "SORT ABECEDY [A] / SORT POPUL�CIE [M/�/P]?\n";
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
                    std::cout << "Zadajte minim�lny po�et obyvate�ov: \n";
                    std::cin >> minResidents;
                    auto filtered = algo.filterWithHasMinResidents<UzemnaJednotka*>(villages.begin(), villages.end(), year, minResidents);
  
                    std::cout << "Chcete pou�i� SORT?[A/N]\n";
                    std::cin >> sortAnswer;
                    if (sortAnswer == "A") {
                        std::cout << "SORT ABECEDY [A] / SORT POPUL�CIE [M/�/P]?\n";
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
                    std::cout << "Neplatn� vo�ba.\n";
                }
            }
            break;
        }

        case 2: { 
            bool secondLevel = true;

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
                        std::cout << "Zadajte re�azec: \n";
                        std::cin >> str;
                        std::cout << "Zadajte rok: \n";
                        std::cin >> year;
                        auto filtered = algo.filterWithContainsStr<UzemnaJednotka*>(begin, end, str);

                        std::cout << "Chcete pou�i� SORT?[A/N]\n";
                        std::cin >> sortAnswer;
                        if (sortAnswer == "A") {
                            std::string typeOfSort;
                            std::cout << "SORT ABECEDY [A] / SORT POPUL�CIE [M/�/P]?\n";
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
                        std::cout << "Zadajte maxim�lny po�et obyvate�ov: \n";
                        std::cin >> maxResidents;
                        auto filtered = algo.filterWithHasMaxResidents<UzemnaJednotka*>(begin, end, year, maxResidents);

                        std::cout << "Chcete pou�i� SORT?[A/N]\n";
                        std::cin >> sortAnswer;
                        if (sortAnswer == "A") {
                            std::string typeOfSort;
                            std::cout << "SORT ABECEDY [A] / SORT POPUL�CIE [M/�/P]?\n";
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
                        std::cout << "Zadajte minim�lny po�et obyvate�ov: \n";
                        std::cin >> minResidents;
                        auto filtered = algo.filterWithHasMinResidents<UzemnaJednotka*>(begin, end, year, minResidents);

                        std::cout << "Chcete pou�i� SORT?[A/N]\n";
                        std::cin >> sortAnswer;
                        if (sortAnswer == "A") {
                            std::string typeOfSort;
                            std::cout << "SORT ABECEDY [A] / SORT POPUL�CIE [M/�/P]?\n";
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
                        std::cout << "[ERROR] Neplatn� vo�ba predik�tu.\n";
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
                    std::cout << "Neplatn� vo�ba.\n";
                }
            }
            break;
        }
        case 3: {
            std::string name;
            int typInput;

            std::cout << "Zadajte n�zov jednotky: \n";
            std::cin >> name;
 
            std::cout << "Zadajte typ (0-ROOT, 1-GEO, 2-REPUBLIKA, 3-REGION, 4-OBEC): \n";
            std::cin >> typInput;

            Typ typ = static_cast<Typ>(typInput);

            ds::adt::ImplicitList<UzemnaJednotka*>* zoznam = nullptr;

            if (loader.getTables().tryFindAll(name, typ, zoznam)) {
                std::cout << "[INFO] N�jden�ch " << zoznam->size() << " jednotiek s n�zvom '" << name << "':\n";

                for (UzemnaJednotka* uj : *zoznam) {
                    std::cout << "---------------------------------\n";
                    uj->printAllYears();  
                }
            }
            else {
                std::cout << "[INFO] Nen�jden� �iadne jednotky s n�zvom '" << name << "'.\n";
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
            std::cout << "Neplatn� vo�ba.\n";
            break;
        }
    }

    navigator.clearHierarchy();
    delete hierarchy;
    return 0;
}
