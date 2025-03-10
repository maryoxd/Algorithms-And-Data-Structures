#include <libds/amt/implicit_sequence.h>
#include <string>
#include "Loader.h"
#include <Windows.h>

int main() {
	ds::amt::ImplicitSequence<int> is;
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
	loader.printCities(2024);



}