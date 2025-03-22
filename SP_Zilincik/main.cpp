#include <string>
#include "Loader.h"
#include <Windows.h>
#include "Algoritmus.h"
#include "Predicates.h"


int main() {
	SetConsoleOutputCP(1250);
	SetConsoleCP(1250);

	Loader loader;
	std::string filename1 = "2020.csv";
	std::string filename2 = "2021.csv";
	std::string filename3 = "2022.csv";
	std::string filename4 = "2023.csv";
	std::string filename5 = "2024.csv";

	std::vector<std::string> filenames = { filename1, filename2, filename3, filename4, filename5 };

	loader.loadCsv(filenames);

	std::vector<Village> Villages = loader.getVillages();
	Algoritmus algo;


	// CUI
	std::cout << "Vitajte v CLI, vyberte si z možností:" << '\n';
	while (true) {
		std::cout << "\n1. [PREDIKÁT 1 - NÁZOV DANEJ OBCE OBSAHUJE ZADANÝ REAZEC] | [reazec]" << '\n';
		std::cout << "2. [PREDIKÁT 2 - CELKOVÝ POÈET OBYVATE¼OV V ZADANOM ROKU BOL <= ZADANÉMU POÈTU] | [rok] + [poèet obyvate¾ov]" << '\n';
		std::cout << "3. [PREDIKÁT 3 - CELKOVÝ POÈET OBYVATE¾OV V ZADANOM ROKU BOL >= ZADANÉMU POÈTU] | [rok] + [poèet obyvate¾ov]" << '\n';
		std::cout << "4. [VYPÍŠ VŠETKY OBCE]" << '\n';
		std::cout << "5. [VYPÍŠ OBCE V ZADANOM ROKU] | [rok]" << '\n';
		std::cout << "6. [KONIEC]" << '\n';
		std::cout << "Vaša možnos:" << '\n';
		int choice;
		std::cin >> choice;

		switch (choice) {
		case 1: {
			std::cout << "Zadajte h¾adaný reazec:" << '\n';
			std::string str;
			std::cin >> str;
			std::cout << "Chcete filtrova aj pod¾a roka? [ano / nie]" << '\n';
			std::string volba;
			std::cin >> volba;
			if (volba == "ano") {
				std::cout << "Zadajte rok:" << '\n';
				int year;
				std::cin >> year;
				std::cout << "Zoznam obcí, ktoré obsahujú reazec [" << str << "] a sú z roku [" << year << "]:" << '\n';

				std::vector<Village> filteredVillage = algo.FilterWithContainsStr(Villages, str, year);
				algo.PrintItems(filteredVillage.begin(), filteredVillage.end());
				size_t size = filteredVillage.size();
				std::cout << "Obce boli vypísané. Poèet: [" << size << "] " << '\n';
				break;
			}
			if (volba == "nie") {
				std::cout << "Zoznam obcí, ktoré obsahujú reazec [" << str << "]:" << '\n';

				std::vector<Village> filteredVillage = algo.FilterWithContainsStr(Villages, str);
				algo.PrintItems(filteredVillage.begin(), filteredVillage.end());
				size_t size = filteredVillage.size();

				std::cout << "Obce boli vypísané. Poèet: [" << size << "] " << '\n';
				break;
			}
			std::cout << "Nesprávna vo¾ba." << '\n';
			break;
		}

		case 2: {
			std::cout << "Zadajte rok:" << '\n';
			int year;
			std::cin >> year;
			std::cout << "Zadajte maximálny poèet obyvate¾ov:" << '\n';
			int maxResidents;
			std::cin >> maxResidents;
			std::cout << "Zoznam obcí, ktoré v roku [" << year << "] majú <= [" << maxResidents << "] obyvate¾ov:" << '\n';

			std::vector<Village> filteredVillage = algo.FilterWithHasMaxResidents(Villages, year, maxResidents);
			algo.PrintItems(filteredVillage.begin(), filteredVillage.end());
			size_t size = filteredVillage.size();

			std::cout << "Obce boli vypísané. Poèet: [" << size << "] " << '\n';
			break;

		}
		case 3: {
			std::cout << "Zadajte rok:" << '\n';
			int year;
			std::cin >> year;
			std::cout << "Zadajte minimálny poèet obyvate¾ov:" << '\n';
			int minResidents;
			std::cin >> minResidents;
			std::cout << "Zoznam obcí, ktoré v roku [" << year << "] majú >= [" << minResidents << "] obyvate¾ov:" << '\n';

			std::vector<Village> filteredVillage = algo.FilterWithHasMinResidents(Villages, year, minResidents);
			algo.PrintItems(filteredVillage.begin(), filteredVillage.end());
			size_t size = filteredVillage.size();

			std::cout << "Obce boli vypísané. Poèet: [" << size << "] " << '\n';
			break;
		}
		case 4: {
			std::cout << "Všetky obce:" << '\n';
			loader.printVillages();

			size_t size = loader.getSize();
			std::cout << "Obce boli vypísané. Poèet: [" << size << "] " << '\n';
			break;
		}
		case 5: {
			std::cout << "Zadajte rok:" << '\n';
			int year;
			std::cin >> year;
			std::cout << "Obce v roku [" << year << "]:" << '\n';
			loader.printVillages(year);

			size_t size = loader.getSize(year);
			std::cout << "Obce boli vypísané. Poèet: [" << size << "] " << '\n';
			break;
		}
		case 6: {
			std::cout << "Ukonèujem program." << '\n';
			return 0;
		}
		default:
			std::cout << "Ukonèujem program,\n";
		}
		
	}
}
