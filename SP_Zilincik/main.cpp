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
	std::cout << "Vitajte v CUI, vyberte si z možností:" << '\n';
	while (true) {
		std::cout << "1. [PREDIKÁT 1 - NÁZOV DANEJ OBCE OBSAHUJE ZADANÝ REAZEC] | [reazec]" << '\n';
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
				std::cout << "Zoznam obcí, ktoré obsahujú reazec [" << str << "]:" << '\n';
				std::vector<City> filteredCities = algo.filter<City>(
					cities.begin(), cities.end(),
				[containsStr, str](const City& city) { return containsStr(city, str); }
					);

				for (const auto& city : filteredCities) {
					city.print();
				}
				
				size_t size = filteredCities.size();
				std::cout << "Obce boli vypísané. Poèet: [" << size << "] " << '\n';
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
					
				std::vector<City> filteredCities = algo.filter<City>(
					cities.begin(), cities.end(),
					[hasMaxResidents, year, maxResidents](const City& city) { return hasMaxResidents(city, year, maxResidents); }
				);

				for (const auto& city : filteredCities) {
					city.print();
				}

				size_t size = filteredCities.size();
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
					
				std::vector<City> filteredCities = algo.filter<City>(
					cities.begin(), cities.end(),
					[hasMinResidents, year, minResidents](const City& city) { return hasMinResidents(city, year, minResidents); }
				);

				for (const auto& city : filteredCities) {
					city.print();
				}

				size_t size = filteredCities.size();
				std::cout << "Obce boli vypísané. Poèet: [" << size << "] " << '\n';
				break;
			}
			case 4: {
				std::cout << "Všetky obce:" << '\n';
				loader.printCities();

				int size = loader.getSize();
				std::cout << "Obce boli vypísané. Poèet: [" << size << "] " << '\n';
				break;
			}
			case 5: {
				std::cout << "Zadajte rok:" << '\n';
				int year;
				std::cin >> year;
				std::cout << "Obce v roku [" << year << "]:" << '\n';
				loader.printCities(year);

				int size = loader.getSize(year);
				std::cout << "Obce boli vypísané. Poèet: [" << size << "] " << '\n';
				break;
			}
			case 6: {
				std::cout << "Ukonèujem program." << '\n';
				return 0;
			}
		default: ;
		}
		std::cout << "\n";
	}

	// CUI

	
}
