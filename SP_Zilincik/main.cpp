#include <libds/amt/implicit_sequence.h>
#include <string>
#include "Loader.h"

int main() {
	ds::amt::ImplicitSequence<int> is;
	Loader loader;
	std::string filename = "2022.csv";
	loader.loadCsv(filename);
	loader.printCities();



}