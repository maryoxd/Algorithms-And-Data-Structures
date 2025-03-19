#include <string>
#include "Loader.h"
#include <Windows.h>
#include "Algoritmus.h"


int main() {
	SetConsoleOutputCP(1250);
	SetConsoleCP(1250);

	// PREDICATES

	auto containsStr = [](const City& city, std::string str) {
		return city.getName().find(str) != std::string::npos;
		};

	auto hasMaxResidents = [](const City& city, int year, int maxResidents) {
		return city.getYear() == year && city.getPopulation() <= maxResidents;
		};

	auto hasMinResidents = [](const City& city, int year, int minResidents) {
		return city.getYear() == year && city.getPopulation() >= minResidents;
		};

	// PREDICATES

	Loader loader;
	std::string filename1 = "2020.csv";
	std::string filename2 = "2021.csv";
	std::string filename3 = "2022.csv";
	std::string filename4 = "2023.csv";
	std::string filename5 = "2024.csv";

	std::vector<std::string> filenames = { filename1, filename2, filename3, filename4, filename5 };

	loader.loadCsv(filenames);
	
	std::vector<City> cities = loader.getCities();
	Algoritmus algo;


	// CUI
	std::cout << "Vitajte v CUI, vyberte si z možností:" << std::endl;
	while (true) {
		std::cout << "1. [PREDIKÁT 1 - NÁZOV DANEJ OBCE OBSAHUJE ZADANÝ REAZEC] | [reazec]" << std::endl;
		std::cout << "2. [PREDIKÁT 2 - CELKOVÝ POÈET OBYVATE¼OV V ZADANOM ROKU BOL <= ZADANÉMU POÈTU] | [rok] + [poèet obyvate¾ov]" << std::endl;
		std::cout << "3. [PREDIKÁT 3 - CELKOVÝ POÈET OBYVATE¾OV V ZADANOM ROKU BOL >= ZADANÉMU POÈTU] | [rok] + [poèet obyvate¾ov]" << std::endl;
		std::cout << "4. [VYPÍŠ VŠETKY OBCE]" << std::endl;
		std::cout << "5. [VYPÍŠ OBCE V ZADANOM ROKU] | [rok]" << std::endl;
		std::cout << "6. [KONIEC]" << std::endl;
		std::cout << "Vaša možnos:" << std::endl;
		int choice;
		std::cin >> choice;

		switch (choice) {
			case 1: {
				std::cout << "Zadajte h¾adaný reazec:" << std::endl;
				std::string str;
				std::cin >> str;
				std::cout << "Zoznam obcí, ktoré obsahujú reazec [" << str << "]:" << std::endl;
				std::vector<City> filteredCities = algo.filter<City>(
					cities.begin(), cities.end(),
				[containsStr, str](const City& city) { return containsStr(city, str); }
					);

				for (const auto& city : filteredCities) {
					city.print();
				}
				
				int size = filteredCities.size();
				std::cout << "Obce boli vypísané. Poèet: [" << size << "] " << std::endl;
				break;	
			}
			case 2: {
				std::cout << "Zadajte rok:" << std::endl;
				int year;
				std::cin >> year;
				std::cout << "Zadajte maximálny poèet obyvate¾ov:" << std::endl;
				int maxResidents;
				std::cin >> maxResidents;
				std::cout << std::endl;
				std::cout << "Zoznam obcí, ktoré v roku [" << year << "] majú maximálne [" << maxResidents << "] obyvate¾ov:" << std::endl;
				std::vector<City> filteredCities = algo.filter<City>(
					cities.begin(), cities.end(),
					[hasMaxResidents, year, maxResidents](const City& city) { return hasMaxResidents(city, year, maxResidents); }
				);

				for (const auto& city : filteredCities) {
					city.print();
				}

				int size = filteredCities.size();
				std::cout << "Obce boli vypísané. Poèet: [" << size << "] " << std::endl;
				break;

			}
			case 3: {
				std::cout << "Zadajte rok:" << std::endl;
				int year;
				std::cin >> year;
				std::cout << "Zadajte minimálny poèet obyvate¾ov:" << std::endl;
				int minResidents;
				std::cin >> minResidents;
				std::cout << "Zoznam obcí, ktoré v roku [" << year << "] majú maximálne [" << minResidents << "] obyvate¾ov:" << std::endl;
				std::vector<City> filteredCities = algo.filter<City>(
					cities.begin(), cities.end(),
					[hasMinResidents, year, minResidents](const City& city) { return hasMinResidents(city, year, minResidents); }
				);

				for (const auto& city : filteredCities) {
					city.print();
				}

				int size = filteredCities.size();
				std::cout << "Obce boli vypísané. Poèet: [" << size << "] " << std::endl;
				break;
			}
			case 4: {
				std::cout << "Všetky obce:" << std::endl;
				loader.printCities();

				int size = loader.getSize();
				std::cout << "Obce boli vypísané. Poèet: [" << size << "] " << std::endl;
				break;
			}
			case 5: {
				std::cout << "Zadajte rok:" << std::endl;
				int year;
				std::cin >> year;
				std::cout << "Obce v roku [" << year << "]:" << std::endl;
				loader.printCities(year);

				int size = loader.getSize(year);
				std::cout << "Obce boli vypísané. Poèet: [" << size << "] " << std::endl;
				break;
			}
			case 6: {
				std::cout << "Ukonèujem program." << std::endl;
				return 0;
			}
		}
		std::cout << "\n";
	}

	// CUI

	
}
