#pragma once

#include <string>

enum class Typ {
	ROOT,
	GEO,
	REPUBLIKA,
	REGION,
	OBEC
};

std::string typToString(Typ typ);
