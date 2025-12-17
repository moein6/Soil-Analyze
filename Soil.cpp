#include "Soil.h"

Soil::Soil(const System sys) : m_D60(0), m_D30(0), m_D10(0), m_CU(0), m_CC(0),m_LL(0),M_PL(0) {

		Set_System(sys);
		m_PI = m_LL - M_PL;

	}//	end 

Soil::Soil(const std::map<double, double> data, const System sys) :m_D60(0), m_D30(0), m_D10(0), m_CU(0), m_CC(0), m_LL(0), M_PL(0)
	{
		Set_System(sys);
		Copy(&data);
	}

void Soil::Add_Data()
	{

		if (!m_Data.empty()) {
			std::cout << "Data is already in\n";
			return;
		}
		
		
		const auto sys = Get_Sieve_System(m_System);
		const auto NO = Get_Sieve_NO(m_System);
		
		std::cout << std::setw(7) << std::left << "#NO" << '|';
		std::cout << std::setw(7) << std::left << " SIZE (mm)" << '|';
		std::cout << std::setw(7) << std::right << " PASSING (%)\n" << std::string(30, '-') << '\n';

		for (int i = 0; i < sys.size(); i++) {

			double percent;

			std::cout << std::setw(7) << std::left << NO.at(i) << "| ";
			std::cout << std::setw(7) << std::left << sys.at(i) << "| ";

			std::cin >> percent;

			if (percent <= 100.0 and percent >= 0.0)
				m_Data[sys.at(i)] = percent;
			else
			{
				std::cerr << "[ERROR] : Invalid input try again\n\a";
				i--;
			}
		}
		bool recive = true;
		do{
		std::cout << "\nInput LL : ";
		std::cin >> m_LL;
	
		std::cout << "\nInput PL : ";
		std::cin >> M_PL;
			
		if(m_LL <= 100 or m_LL >= 0 or M_PL <= 100 or M_PL >= 0)
			recive = false;
		else
			std::cerr << "[ERROR] : m_LL or m_PL has wrong value!\n";
			
		}while(recive == true);
		
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
	if (filename.empty()) {
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

	//	Skip the header
	std::getline(in, line);
	line.clear();

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

	std::sort(vec.begin(), vec.end(), [](const auto& a, const auto& b) {return std::get<0>(a) > std::get<0>(b); });

	for (auto v : vec)
		m_Data[std::get<0>(v)] = std::get<1>(v);

	std::cout << "\nInput LL : ";
	std::cin >> m_LL;

	std::cout << "\nInput PL : ";
	std::cin >> M_PL;

	m_PI = m_LL - M_PL;




	Calculate_CuCC();
}

void Soil::Print_Data()
	{
		Classify();
		std::system("cls");
	
		std::cout << std::setw(10) << "System" << std::setw(10) << std::right << "Classify\n" << std::string(20, '-') << '\n';

		switch (m_System) {
		case System::ASHTO: std::cout << std::setw(10) << std::left << "[AASHTO]"	<< std::setw(10) << std::right << m_ASHTO_Name	<< '\n';	break;
		case System::ASTM:	std::cout << std::setw(10) << std::left << "[ASTM]"		<< std::setw(10) << std::right << m_ASTM_Name	<< '\n';	break;
		case System::MIT:	std::cout << std::setw(10) << std::left << "[MIT]"		<< std::setw(10) << std::right << m_MIT_Name	<< '\n';	break;
		case System::BSCS:	std::cout << std::setw(10) << std::left << "[BSCS]"		<< std::setw(10) << std::right << m_BSCS_Name	<< '\n';	break;
		case System::ALL: {
			std::cout << std::setw(10) << std::left << "[AASHTO]" << std::setw(10) << std::right << m_ASHTO_Name << '\n';
			std::cout << std::setw(10) << std::left << "[ASTM]" << std::setw(10) << std::right << m_ASTM_Name << '\n';
			std::cout << std::setw(10) << std::left << "[MIT]" << std::setw(10) << std::right << m_MIT_Name << '\n';
		}//	end ALL
		}//	end switch


		std::cout << "\n\n";

		std::cout.precision(4);
		std::cout << std::setw(20) << std::left << "Sieve (mm)" << std::setw(10) << std::right<< "Passing(%)" << '\n' << std::string(30, '-') << '\n';
	
		for (const auto &m : m_Data)
			std::cout << std::setw(15) << std::left << m.first << std::setw(10) << std::right << m.second << '\n';
	
		std::cout << "\n\n";

		std::cout << std::setw(10) << std::left << "LL" << std::setw(10) << std::left << "PL" << std::setw(10) << std::left << "PI" << '\n' << std::string(25, '-') << '\n';
		std::cout << std::setw(10) << std::left << m_LL << std::setw(10) << std::left << M_PL << std::setw(10) << std::left << m_PI << '\n';
	
		std::cout << "\n\n";

		std::cout << std::setw(10) << std::left << "D10" << std::setw(10) << std::left << "D30" << std::setw(10) << std::left << "D60";
		std::cout << std::setw(10) << std::left << "CU" << std::setw(10) << std::left << "CC" << '\n' << std::string(50, '-') << '\n';
	
		std::cout << std::setw(10) << std::left << m_D10 << std::setw(10) << std::left << m_D30 << std::setw(10) << std::left << m_D60;
		std::cout << std::setw(10) << std::left << m_CU << std::setw(10) << std::left << m_CC << '\n';

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

void Soil::Help() {
	std::system("cls");

	std::cout << "\n========== SoilAnalyzer Help ==========\n\n";

	std::cout << "Usage Options:\n";
	std::cout << "--------------------------------------\n";
	std::cout << "1) Insert sieve analysis data from file\n";
	std::cout << "2) Select sieve system (BSCS / ASTM)\n";
	std::cout << "3) Analyze soil classification\n";
	std::cout << "4) Show calculated parameters (D10, D30, D60, CU, CC)\n\n";

	std::cout << "--------------------------------------\n";
	std::cout << "File Input Format (IMPORTANT):\n";
	std::cout << "--------------------------------------\n";
	std::cout << "Data must be provided as:\n";
	std::cout << "    Sieve Size (mm)   Percent Passing (%)\n\n";

	std::cout << "Example:\n";
	std::cout << "    4.75   100\n";
	std::cout << "    2.00    92\n";
	std::cout << "    0.425   60\n";
	std::cout << "    0.075   12\n\n";

	std::cout << "--------------------------------------\n";
	std::cout << "Supported Sieve Systems:\n";
	std::cout << "--------------------------------------\n";

	std::cout << "BSCS Sieves (mm):\n";
	std::cout << "    19 , 9.5 , 4.75 , 2.0 , 0.85 , 0.425 , 0.25 , 0.15 , 0.075\n\n";

	std::cout << "ASTM Sieves (mm):\n";
	std::cout << "    19 , 12.5 , 9.5 , 4.75 , 2.0 , 0.85 , 0.425 , 0.075\n\n";

	std::cout << "--------------------------------------\n";
	std::cout << "Notes:\n";
	std::cout << "--------------------------------------\n";
	std::cout << "- Sieve sizes must be in millimeters (mm)\n";
	std::cout << "- Percent passing must be between 0 and 100\n";
	std::cout << "- Data can be unordered; sorting is handled internally\n";
	std::cout << "- At least two data points are required for interpolation\n\n";

	std::cout << "======================================\n\n";


}

//=====================	private ========================

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

std::array<double, sieveCount> Soil::Get_Sieve_System(const System &sys) {
		return (sys == System::BSCS) ? Sieve::System::BSCS : Sieve::System::ASTM;
	}

std::array<int, sieveCount> Soil::Get_Sieve_NO(const System& sys) {
		return (sys == System::BSCS) ? Sieve::NO::BSCS : Sieve::NO::ASTM;
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

void Soil::Name_ASTM() {
		if (!ValidateData(&m_Data)) return;

		double P4 = 0.0, P200 = 0.0;
		for (const auto& it : m_Data) {
			if (std::abs(it.first - 4.75) < 1e-3)   P4 = it.second;
			if (std::abs(it.first - 0.075) < 1e-3)  P200 = it.second;
		}
		
		
		double A_line = 0.73 * (m_LL - 20.0);


		if (P200 >= 50.0) {
			if (m_LL <= 20) {
				m_ASTM_Name = "ML or OL";
				return;
			}


			if (m_LL < 50) {
				if (m_PI >= A_line) {
					if (m_PI >= 7) m_ASTM_Name = "CL";
					else if (m_PI >= 4) m_ASTM_Name = "CL-ML";
					else m_ASTM_Name = "ML";
				}
				else {
					if (m_PI < 4) m_ASTM_Name = "ML";
					else if (m_PI <= 7) m_ASTM_Name = "CL-ML";
					else m_ASTM_Name = "CL";
				}
			}
			else {
				if (m_PI >= A_line) {
					m_ASTM_Name = "CH";
				}
				else {
					m_ASTM_Name = "MH";
				}
			}
			return;
		}

		bool isSand = (P4 >= 50.0);
		std::string primary = isSand ? "S" : "G";

		if (isSand) {
			primary += (m_CU >= 6.0 && m_CC >= 1.0 && m_CC <= 3.0) ? "W" : "P";
		}
		else {
			primary += (m_CU >= 4.0 && m_CC >= 1.0 && m_CC <= 3.0) ? "W" : "P";
		}

		std::string finesType = "M";  
		if (m_LL > 20) {
			finesType = (m_PI >= A_line) ? "C" : "M";
		}

		if (P200 < 5.0) {
			m_ASTM_Name = primary;
		}
		else if (P200 <= 12.0) {
			m_ASTM_Name = primary + "-" + primary[0] + finesType;
		}
		else {
			m_ASTM_Name = std::string(1, primary[0]) + finesType;
		}
	}

void Soil::Name_AASHTO() {
		if (!ValidateData(&m_Data)) return;

		double P10 = 0.0, P40 = 0.0, P200 = 0.0;
		for (const auto& it : m_Data) {
			if (std::abs(it.first - 2.00) < 1e-3)   P10 = it.second;
			if (std::abs(it.first - 0.425) < 1e-3)  P40 = it.second;
			if (std::abs(it.first - 0.075) < 1e-3)  P200 = it.second;
		}

		if (P200 <= 35) {
			if (P200 <= 15 && P40 <= 30 && P10 <= 50) {
				m_ASHTO_Name = "A-1-a";
				// m_ASHTO_Description = "Stone fragments, gravel and sand";
				return;
			}

			if (P200 <= 25 && P40 <= 50) {
				m_ASHTO_Name = "A-1-b";
				//m_ASHTO_Description = "Gravelly sand";
				return;
			}

			if (P200 <= 10 && P40 <= 50 && P10 >= 50) {
				m_ASHTO_Name = "A-3";
				//m_ASHTO_Description = "Fine sand";
				return;
			}

			m_ASHTO_Name = "A-2";
			if (m_LL < 40) {
				if (m_PI < 10) {
					m_ASHTO_Name += "-4";
					//m_ASHTO_Description = "Silty or clayey gravel and sand";
				}
				else {
					m_ASHTO_Name += "-6";
					//m_ASHTO_Description = "Clayey gravel and sand";
				}
			}
			else {
				if (m_PI < 10) {
					m_ASHTO_Name += "-5";
					//m_ASHTO_Description = "Silty or clayey gravel and sand (high LL)";
				}
				else {
					m_ASHTO_Name += "-7";
					//m_ASHTO_Description = "Clayey gravel and sand (high LL)";
				}
			}
			return;
		}

		else {
			if (m_LL < 40) {
				if (m_PI < 10) {
					m_ASHTO_Name = "A-4";
					//m_ASHTO_Description = "Silty soil";
				}
				else {
					m_ASHTO_Name = "A-6";
					//m_ASHTO_Description = "Clayey soil";
				}
			}
			else {
				if (m_PI < 10) {
					m_ASHTO_Name = "A-5";
					//m_ASHTO_Description = "Silty soil (high LL)";
				}
				else {
					if (m_PI <= (m_LL - 30)) {
						m_ASHTO_Name = "A-7-5";
						//m_ASHTO_Description = "Clayey soil (PI ≤ LL-30)";
					}
					else {
						m_ASHTO_Name = "A-7-6";
						//m_ASHTO_Description = "Clayey soil (PI > LL-30)";
					}
				}
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

		if (percentFines < 50) {
			if (percentGravel > percentSand)
				m_MIT_Name = "Gravelly soil";
			else
				m_MIT_Name = "Sandy soil";
		}
		else {
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
			if (std::abs(it.first - 0.063) < 1e-3)
				percentFines = it.second;

		double percentGravel = 0.0, percentSand = 0.0;
		for (const auto& it : m_Data) {
			if (it.first > 4.75) percentGravel += it.second;
			else if (it.first <= 4.75 && it.first > 0.063) percentSand += it.second;
		}

		if (percentFines < 35) {
			std::string symbol = (percentGravel > percentSand) ? "G" : "S";

			if (percentFines < 5) {
				if (symbol == "G") {
					symbol += (m_CU >= 4 && m_CC >= 1 && m_CC <= 3) ? "W" : "P";
				}
				else {
					symbol += (m_CU >= 6 && m_CC >= 1 && m_CC <= 3) ? "W" : "P";
				}
			}
			else {
				std::string finesType = "M";
				if (m_LL > 20) {
					double A_line = 0.73 * (m_LL - 20);
					finesType = (m_PI >= A_line) ? "C" : "M";
				}
				symbol += finesType;
			}
			m_BSCS_Name = symbol;
		}
	}

void Soil::SaveFile_CSV()
	{
		const std::string Filename = "D:\\Soil_" + get_System_STR() + ".csv";

		std::ofstream out(Filename.data());
		
		out << std::setw(10) << std::left << "System" << std::setw(10) << std::right << "Classify\n" << std::string(20, '-') << '\n';

		switch (m_System) {
		case System::ASHTO: out << std::setw(10) << std::left << "[AASHTO]" << std::setw(10) << std::right << m_ASHTO_Name << '\n';	break;
		case System::ASTM:	out << std::setw(10) << std::left << "[ASTM]" << std::setw(10) << std::right << m_ASTM_Name << '\n';	break;
		case System::MIT:	out << std::setw(10) << std::left << "[MIT]" << std::setw(10) << std::right << m_MIT_Name << '\n';	break;
		case System::BSCS:	out << std::setw(10) << std::left << "[BSCS]" << std::setw(10) << std::right << m_BSCS_Name << '\n';	break;
		case System::ALL: {
			out << std::setw(10) << std::left << "[AASHTO]" << std::setw(10) << std::right << m_ASHTO_Name << '\n';
			out << std::setw(10) << std::left << "[ASTM]" << std::setw(10) << std::right << m_ASTM_Name << '\n';
			out << std::setw(10) << std::left << "[MIT]" << std::setw(10) << std::right << m_MIT_Name << '\n';
		}//	end ALL
		}//	end switch

		out << "\n\n";

		out << std::setw(20) << std::left << "Sieve (mm)" << std::setw(10) << std::right << "Passing(%)" << '\n' << std::string(30, '-') << '\n';


		const auto sys = Get_Sieve_System(m_System);
		const auto NO = Get_Sieve_NO(m_System);

		out << std::setw(7) << std::left << "#NO" << '|';
		out << std::setw(20) << std::left << " SIZE (mm)" << '|';
		out << std::setw(10) << std::right << " PASSING (%)\n" << std::string(30, '-') << '\n';

		short counter = 0;
		
		for (const auto& m : m_Data) {
			out << std::setw(7) << std::left << NO.at(counter) << "| ";
			out << std::setw(20) << std::left << m.first << '|' << std::setw(10) << std::right << m.second << '\n';
			counter++;
		}


		out << "\n\n";
		
		out << std::setw(10) << std::left << "LL" << std::setw(10) << std::left << "PL" << std::setw(10) << std::left << "PI" << '\n' << std::string(25, '-') << '\n';
		out << std::setw(10) << std::left << m_LL << std::setw(10) << std::left << M_PL << std::setw(10) << std::left << m_PI << '\n';
		
		out << "\n\n";
		
		out << std::setw(10) << std::left << "D10" << std::setw(10) << std::left << "D30" << std::setw(10) << std::left << "D60";
		out << std::setw(10) << std::left << "CU" << std::setw(10) << std::left << "CC" << '\n' << std::string(50, '-') << '\n';
		
		out << std::setw(10) << std::left << m_D10 << std::setw(10) << std::left << m_D30 << std::setw(10) << std::left << m_D60;
		out << std::setw(10) << std::left << m_CU << std::setw(10) << std::left << m_CC << '\n';

		out.close();


		std::cout << "Data saved in " << Filename << '\n';
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
	
