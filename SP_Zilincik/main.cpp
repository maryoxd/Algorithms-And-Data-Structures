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
	std::cout << "Vitajte v CUI, vyberte si z mo�nost�:" << std::endl;
	while (true) {
		std::cout << "1. [PREDIK�T 1 - N�ZOV DANEJ OBCE OBSAHUJE ZADAN� RE�AZEC] | [re�azec]" << std::endl;
		std::cout << "2. [PREDIK�T 2 - CELKOV� PO�ET OBYVATE�OV V ZADANOM ROKU BOL <= ZADAN�MU PO�TU] | [rok] + [po�et obyvate�ov]" << std::endl;
		std::cout << "3. [PREDIK�T 3 - CELKOV� PO�ET OBYVATE�OV V ZADANOM ROKU BOL >= ZADAN�MU PO�TU] | [rok] + [po�et obyvate�ov]" << std::endl;
		std::cout << "4. [VYP͊ V�ETKY OBCE]" << std::endl;
		std::cout << "5. [VYP͊ OBCE V ZADANOM ROKU] | [rok]" << std::endl;
		std::cout << "6. [KONIEC]" << std::endl;
		std::cout << "Va�a mo�nos�:" << std::endl;
		int choice;
		std::cin >> choice;

		switch (choice) {
			case 1: {
				std::cout << "Zadajte h�adan� re�azec:" << std::endl;
				std::string str;
				std::cin >> str;
				std::cout << "Zoznam obc�, ktor� obsahuj� re�azec [" << str << "]:" << std::endl;
				std::vector<City> filteredCities = algo.filter<City>(
					cities.begin(), cities.end(),
				[containsStr, str](const City& city) { return containsStr(city, str); }
					);

				for (const auto& city : filteredCities) {
					city.print();
				}
				
				int size = filteredCities.size();
				std::cout << "Obce boli vyp�san�. Po�et: [" << size << "] " << std::endl;
				break;	
			}
			case 2: {
				std::cout << "Zadajte rok:" << std::endl;
				int year;
				std::cin >> year;
				std::cout << "Zadajte maxim�lny po�et obyvate�ov:" << std::endl;
				int maxResidents;
				std::cin >> maxResidents;
				std::cout << std::endl;
				std::cout << "Zoznam obc�, ktor� v roku [" << year << "] maj� maxim�lne [" << maxResidents << "] obyvate�ov:" << std::endl;
				std::vector<City> filteredCities = algo.filter<City>(
					cities.begin(), cities.end(),
					[hasMaxResidents, year, maxResidents](const City& city) { return hasMaxResidents(city, year, maxResidents); }
				);

				for (const auto& city : filteredCities) {
					city.print();
				}

				int size = filteredCities.size();
				std::cout << "Obce boli vyp�san�. Po�et: [" << size << "] " << std::endl;
				break;

			}
			case 3: {
				std::cout << "Zadajte rok:" << std::endl;
				int year;
				std::cin >> year;
				std::cout << "Zadajte minim�lny po�et obyvate�ov:" << std::endl;
				int minResidents;
				std::cin >> minResidents;
				std::cout << "Zoznam obc�, ktor� v roku [" << year << "] maj� maxim�lne [" << minResidents << "] obyvate�ov:" << std::endl;
				std::vector<City> filteredCities = algo.filter<City>(
					cities.begin(), cities.end(),
					[hasMinResidents, year, minResidents](const City& city) { return hasMinResidents(city, year, minResidents); }
				);

				for (const auto& city : filteredCities) {
					city.print();
				}

				int size = filteredCities.size();
				std::cout << "Obce boli vyp�san�. Po�et: [" << size << "] " << std::endl;
				break;
			}
			case 4: {
				std::cout << "V�etky obce:" << std::endl;
				loader.printCities();

				int size = loader.getSize();
				std::cout << "Obce boli vyp�san�. Po�et: [" << size << "] " << std::endl;
				break;
			}
			case 5: {
				std::cout << "Zadajte rok:" << std::endl;
				int year;
				std::cin >> year;
				std::cout << "Obce v roku [" << year << "]:" << std::endl;
				loader.printCities(year);

				int size = loader.getSize(year);
				std::cout << "Obce boli vyp�san�. Po�et: [" << size << "] " << std::endl;
				break;
			}
			case 6: {
				std::cout << "Ukon�ujem program." << std::endl;
				return 0;
			}
		}
		std::cout << "\n";
	}

	// CUI

	
}
