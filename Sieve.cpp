#include "Sieve.h"


Sieve::Sieve() {
	No = -1;
	Passing = Size = -1.0;
}

Sieve::Sieve(const int& newno, const double& newsize, const double& newpassing) {
	No = (newno >= 0 && newno <= 200) ? newno : -1;
	Size = (newsize >= 0 && newsize < 10) ? newsize : -1.0;
	Passing = (newpassing > 0 && newpassing <= 100) ? newpassing : -1.0;
}


std::string Sieve::get_System_STR(const System& sys)const noexcept {
	switch (sys) {
	case System::AASHTO: return "ASSHTO";	break;
	case System::MIT:	return "MIT";		break;
	case System::BSCS:	return "BSCS";		break;
	case System::ASTM:	return "ASTM";		break;
	case System::ALL:	return "US";		break;
	}
	return "Unknown";
}

void Sieve::Reset_cin()const {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}


