#include "Soil.h"

Soil::Soil(const System sys) : m_D60(0), m_D30(0), m_D10(0), m_CU(0), m_CC(0),m_LL(0),M_PL(0) {


	Set_System(sys);
	m_PI = m_LL - M_PL;



}//	end 

Soil::Soil(const std::map<double, double> data, const System sys) :m_D60(0), m_D30(0), m_D10(0), m_CU(0), m_CC(0), m_LL(0), M_PL(0)
{
	Set_System(sys);
	Copy(&data);
	Calculate_CuCC();
}

void Soil::Set_System(const System sys) {
	short choose;

	if (sys == System::NUN) {

		std::cout << "Select your naming system\n";

		std::cout << "1 - ASSHTO\n2 - MIT \n3 - BSCS \n4 - ASTM / USCS\n5 - ALL US System (ASSHTO, MIT, ASTM/USCS) \n -> ";
		std::cin >> choose;

		switch (choose) {
		case 1: m_System = System::ASHTO;	break;
		case 2: m_System = System::MIT;		break;
		case 3: m_System = System::BSCS;	break;
		case 4: m_System = System::ASTM;	break;
		case 5:	m_System = System::ALL;		break;
		default:
			std::cout << "Wrong input default system : ASTM ";
			m_System = System::ASTM;
		}//	end switch
	}
	else
		m_System = sys;

}//	end function

void Soil::Copy(const std::map<double, double>* data)
{
	m_Data = *data;
}

bool Soil::ValidateData(const std::map<double, double>* data) const
{
	if (data->empty()) {
		std::cerr << "Error: No data found!\n";
		return false;
	}
	if (data->size() < 3) {
		std::cerr << "Error: Not enough sieves data to caculate cc and cu.\n";
		return false;
	}
	return true;
}

bool Soil::ValidateData(const Soil* soil) const
{
	return ValidateData(&soil->m_Data);
}

void Soil::Add_Data()
{

	if (!m_Data.empty()) {
		std::cout << "Data is already in\n";
		return;
	}

	
	const auto arr = (m_System == System::MIT or m_System == System::ASTM or m_System == System::ASHTO or m_System == System::ALL)
		? Sieve_System::ASTM_Sieves : Sieve_System::BS_Sieves;


	for (int i = 0; i < arr.size(); i++)
	{

		double percent;

//		if (m_System != System::BSCS)
//			std::cout << std::setw(3) << std::right << ASTM_SeivesNo.at(i) << " : ";
//		else
			std::cout << std::setw(3) << std::right << arr.at(i) << " : ";

		std::cin >> percent;
		if (percent < 100 or percent > 0)
			m_Data[arr.at(i)] = percent;
		else
		{
			std::cerr << "[ERROR] : Invalid input try again\n\a";
			i--;
		}
	}


	std::cout << "\nInput LL : ";
	std::cin >> m_LL;
	
	std::cout << "\nInput PL : ";
	std::cin >> M_PL;


	m_PI = m_LL - M_PL;


	if (ValidateData(&m_Data))
		Calculate_CuCC();
	else
		std::cerr << "[ERROR] : data is invalid\n\a";

}

void Soil::InsertData(std::string filename)
{

	if (!m_Data.empty()) {
		std::cerr << "Data is already in\n\a";
		return;
	}

	if (filename == null) {
		std::cout << "Enter file path (csv/txt): ";
		std::cin >> filename;
	}

	std::ifstream in(filename);
	if (!in.is_open()) {
		std::cerr << "Error: Cannot open file " << filename << "\n";
		return;
	}

	std::vector<std::tuple<double, double>> vec;

	std::string line;
	while (std::getline(in, line))
	{
		std::replace(line.begin(), line.end(), ',', ' ');
		std::stringstream ss(line);
		double sieve;
		double percent;
		ss >> sieve >> percent;
		vec.push_back({ sieve , percent });
		//	m_Data[sieve] = percent;
	}

	in.close();

	std::sort(vec.begin(), vec.end(), [](const auto& a, const auto& b) {return std::get<0>(a) > std::get<1>(b); });

	for (auto v : vec)
		m_Data[std::get<0>(v)] = std::get<1>(v);

	std::cout << "\nInput LL : ";
	std::cin >> m_LL;

	std::cout << "\nInput PL : ";
	std::cin >> M_PL;

	m_PI = m_LL - M_PL;




	Calculate_CuCC();
}

void Soil::Calculate_CuCC()
{
	if (!ValidateData(&m_Data)) return;

	// compute approximate D10, D30, D60 by interpolation
	std::vector<std::pair<double, double>> arr;
	for (const auto& it : m_Data)
		arr.emplace_back((double)it.first, it.second);

	std::sort(arr.begin(), arr.end(),
		[](auto& a, auto& b) { return a.first > b.first; });

	auto interp = [&](double target) -> double {
		for (size_t i = 1; i < arr.size(); i++) {
			if (arr[i - 1].second >= target && arr[i].second <= target) {
				double x1 = arr[i - 1].first, y1 = arr[i - 1].second;
				double x2 = arr[i].first, y2 = arr[i].second;
				return x1 + (x2 - x1) * (target - y1) / (y2 - y1);
			}
		}
		return 0.0;
		};

	m_D10 = interp(10);
	m_D30 = interp(30);
	m_D60 = interp(60);

	Calculate_CU();
	Calculate_CC();
}

void Soil::Calculate_CU()
{
	if (m_D10 != 0)
		m_CU = m_D60 / m_D10;
	else
		m_CU = 0;
}

void Soil::Calculate_CC()
{
	if (m_D60 * m_D10 != 0)
		m_CC = (m_D30 * m_D30) / (m_D60 * m_D10);
	else
		m_CC = 0;
}

void Soil::Print_Data()
{
	constexpr auto header_sieve = "{:<10}{:>10}\n";								//	sieve , pass
	constexpr auto header_sieve_Body = "{:<10}{:>10}%\n";								//	sieve , pass
	constexpr auto header_Atterburg = "{:<10.4f},{:<10.4f},{:<10.4f}\n";						//	atterburg
	constexpr auto header_DC= "{:<10},{:<10.4f},{:<10.4f},{:<10.4f},{:<10.4f}\n";		//	d , c
	constexpr auto header_Name = "{:<10},{:>10}\n";							//	name
	constexpr auto Header_NameBody = "[{}] : {:>10}\n";


	std::cout << std::format(header_Name, "system", "Classify") << std::string(15, '-') << '\n';

	switch (m_System) {
		case System::ASHTO:	std::cout << std::format(Header_NameBody, "ASSHTO", m_ASHTO_Name);		break;
		case System::ASTM:	std::cout << std::format(Header_NameBody, "ASTM", m_ASTM_Name);			break;
		case System::BSCS:	std::cout << std::format(Header_NameBody, "BSCD", m_BSCS_Name);			break;
		case System::MIT:	std::cout << std::format(Header_NameBody, "MIT", m_MIT_Name);			break;
		case System::ALL: {
			std::cout << std::format(Header_NameBody, "ASSHTO", m_ASHTO_Name);
			std::cout << std::format(Header_NameBody, "ASTM", m_ASTM_Name);
			std::cout << std::format(Header_NameBody, "BSCD", m_BSCS_Name);
			std::cout << std::format(Header_NameBody, "MIT", m_MIT_Name);
		}																							
	}//	end switch


	std::cout << "\n\n";

	std::cout << std::format(header_sieve, "Sieve", "Passing") << std::string(20, '-') << '\n';
	
	for (auto m : m_Data)
		std::cout << std::format(header_sieve_Body, m.first, m.second);

	std::cout << "\n\n";

	std::cout << std::format(header_Atterburg, "LL", "PL", "PI") << std::string(30,'-') << '\n';
	std::cout << std::format(header_Atterburg, m_LL, M_PL, m_PI);

	std::cout << "\n\n";

	std::cout << std::format(header_DC, "D10", "D30", "D60", "CU", "CC") << std::string(50, '-') << '\n';
	std::cout << std::format(header_DC, m_D10, m_D30, m_D60, m_CU, m_CC);



}

void Soil::Name_ASTM()
{
	if (!ValidateData(&m_Data)) return;

	double percentFines = 0.0; // دانه‌های کمتر از 0.075
	for (const auto& it : m_Data)
		if (std::abs(it.first - 0.075) < 1e-3)
			percentFines = it.second;

	// خاک عمدتاً ماسه/سنگریزه
	if (percentFines < 50) {
		double percentGravel = 0.0;
		double percentSand = 0.0;

		for (const auto& it : m_Data) {
			if (it.first > 4.75) percentGravel += it.second;
			else if (it.first <= 4.75 && it.first > 0.075) percentSand += it.second;
		}

		if (percentGravel > percentSand) { // خاک درشت غالب
			if (m_CU > 4 && m_CC > 1 && m_CC < 3)
				m_ASTM_Name = "GW (Well-graded gravel)";
			else
				m_ASTM_Name = "GP (Poorly-graded gravel)";
		}
		else { // ماسه غالب
			if (m_CU > 6 && m_CC > 1 && m_CC < 3)
				m_ASTM_Name = "SW (Well-graded sand)";
			else
				m_ASTM_Name = "SP (Poorly-graded sand)";
		}
	}
	// خاک ریز غالب
	else {
		if (m_LL < 50) {
			if (m_PI < 4) m_ASTM_Name = "ML (Silt - low plasticity)";
			else if (m_PI > 7) m_ASTM_Name = "CL (Clay - low plasticity)";
			else m_ASTM_Name = "CL-ML (Silty clay)";
		}
		else {
			if (m_PI < 4) m_ASTM_Name = "MH (Elastic silt)";
			else if (m_PI > 7) m_ASTM_Name = "CH (Fat clay)";
			else m_ASTM_Name = "CH-MH (Intermediate fine soil)";
		}
	}
}

void Soil::Name_AASHTO()
{
	if (!ValidateData(&m_Data)) return;

	double passNo10 = 0, passNo40 = 0, passNo200 = 0;
	for (const auto& it : m_Data) {
		if (std::abs(it.first - 2.00) < 1e-3) passNo10 = it.second;
		if (std::abs(it.first - 0.425) < 1e-3) passNo40 = it.second;
		if (std::abs(it.first - 0.075) < 1e-3) passNo200 = it.second;
	}

	// دانه ریز کمتر از 35٪ → خاک دانه‌ای
	if (passNo200 < 35) {
		if (passNo10 > 50)
			m_ASHTO_Name = "A-1-b (Gravelly sand)";
		else
			m_ASHTO_Name = "A-3 (Sandy or gravelly soil)";
	}
	else { // خاک ریز غالب
		if (m_LL < 40) {
			if (m_PI < 10)
				m_ASHTO_Name = "A-4 (Silt)";
			else
				m_ASHTO_Name = "A-6 (Clay)";
		}
		else {
			if (m_PI < 10)
				m_ASHTO_Name = "A-5 (Elastic silt)";
			else
				m_ASHTO_Name = "A-7 (Clay of high plasticity)";
		}
	}
}

void Soil::Name_MIT()
{
	if (!ValidateData(&m_Data)) return;

	double percentFines = 0.0;
	for (const auto& it : m_Data)
		if (std::abs(it.first - 0.075) < 1e-3)
			percentFines = it.second;

	double percentGravel = 0.0, percentSand = 0.0;
	for (const auto& it : m_Data) {
		if (it.first > 4.75) percentGravel += it.second;
		else if (it.first <= 4.75 && it.first > 0.075) percentSand += it.second;
	}

	if (percentFines < 50) { // خاک دانه‌ای
		if (percentGravel > percentSand)
			m_MIT_Name = "Gravelly soil";
		else
			m_MIT_Name = "Sandy soil";
	}
	else { // خاک ریز
		if (m_LL < 35)
			m_MIT_Name = "Silt or clay of low plasticity";
		else
			m_MIT_Name = "Silt or clay of high plasticity";
	}
}

void Soil::Name_BSCS()
{
	if (!ValidateData(&m_Data)) return;

	double percentFines = 0.0;
	for (const auto& it : m_Data)
		if (std::abs(it.first - 0.075) < 1e-3)
			percentFines = it.second;

	double percentGravel = 0.0, percentSand = 0.0;
	for (const auto& it : m_Data) {
		if (it.first > 4.75) percentGravel += it.second;
		else if (it.first <= 4.75 && it.first > 0.075) percentSand += it.second;
	}

	if (percentFines < 35) { // خاک درشت
		if (percentGravel > percentSand)
			m_BSCS_Name = "Gravelly soil";
		else
			m_BSCS_Name = "Sandy soil";
	}
	else { // خاک ریز
		if (m_LL < 35) {
			if (m_PI < 10)
				m_BSCS_Name = "Silty clay (low plasticity)";
			else
				m_BSCS_Name = "Clayey silt (moderate plasticity)";
		}
		else {
			if (m_PI < 10)
				m_BSCS_Name = "Silt (high LL)";
			else
				m_BSCS_Name = "Clay (high LL)";
		}
	}
}

void Soil::Classify()
{
	switch (m_System) {
	case System::ASTM:	Name_ASTM();	break;
	case System::ASHTO:	Name_AASHTO();	break;
	case System::MIT:	Name_MIT();		break;
	case System::BSCS:	Name_BSCS();	break;
	case System::ALL: {
		Name_ASTM();
		Name_AASHTO();
		Name_MIT();
	}			break;	
	default:
		std::cout << "[!] Invalid input, defaulting to ASTM\n";
		m_System = System::ASTM;
		Name_ASTM();
		break;
	}//	end switch

}//	end function

void Soil::SaveFile_CSV()
{
	const std::string Filename = "D:\\Soil_" + get_System_STR() + ".csv";

	std::ofstream out(Filename.data());

	for (const auto& it : m_Data)
		out << it.first << "," << it.second << "\n";

	switch (m_System) {
	case System::ASHTO: out << '[' << get_System_STR() << "] " << m_ASHTO_Name;	break;
	case System::ASTM:	out << '[' << get_System_STR() << "] " << m_ASTM_Name;		break;
	case System::MIT:	out << '[' << get_System_STR() << "] " << m_MIT_Name;		break;
	case System::BSCS:	out << '[' << get_System_STR() << "] " << m_BSCS_Name;		break;
	case System::ALL: {
		out << '[' << get_System_STR() << "] " << m_ASHTO_Name;
		out << '[' << get_System_STR() << "] " << m_ASTM_Name;
		out << '[' << get_System_STR() << "] " << m_MIT_Name;
		out << '[' << get_System_STR() << "] " << m_BSCS_Name;
	}//	end case
	}//	end switch

	out.close();

	std::cout << "Data saved to " << Filename << "\n";
}

std::string Soil::get_System_STR()const noexcept{
	switch (m_System) {
	case System::ASHTO: return "ASSHTO";	break;
	case System::MIT:	return "MIT";		break;
	case System::BSCS:	return "BSCS";		break;
	case System::ASTM:	return "ASTM";		break;
	case System::ALL:	return "US";		break;
	}
	return "Unknown";
}

