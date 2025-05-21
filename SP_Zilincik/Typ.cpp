#include "Typ.h"

std::string typToString(Typ typ)
{
	switch (typ) {
	case Typ::ROOT: return "ROOT";
	case Typ::GEO: return "GEO";
	case Typ::REPUBLIKA: return "REPUBLIKA";
	case Typ::REGION: return "REGION";
	case Typ::OBEC: return "OBEC";
	default: return "NEZNÁMY";
	}
}
