#include "Insert.h"

Insert::Insert() {
	m_data.clear();
	m_system = System::NUN;
}

void Insert::Sort() {

	std::ranges::sort(m_data, {}, &Sieve::No);

	//std::sort(m_data.begin(), m_data.end(), [](const auto& a, const auto& b) {return a.No > b.No; });
}

void Insert::Print() {
	constexpr auto Header_Grain = "{:^5}|{:^7}|{:^7}\n";

	std::cout << std::format(Header_Grain, "NO", "SIZE", "PASS") << std::string('-', 40) << '\n';

	for (const auto& [no, size, pass] : m_data)
		std::cout << std::format(Header_Grain, no, size, pass);
}

bool Insert::Check_DIR() {
	
	auto make = [](std::filesystem::path p) {
		if (!std::filesystem::exists(p))
			if (!std::filesystem::create_directories(p))
				throw(std::filesystem::filesystem_error("Failed to create directory ", p, std::make_error_code(std::errc::io_error)));
	};

	try {
		make(Root);
		make(History);
		make(Data);

	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cout << e.what() << "\n\a";
		return false;
	}

	return true;
}

void Insert::Reset_cin()const {
	Sieve sieve;
	sieve.Reset_cin();
}

void Insert::Set_system() {
	Analyze analyze;
	char choose;
	constexpr auto fmt = "System is detected as : {}\nDo you want to change it ?(y/n)";

	m_system = analyze.AnalyzeData(m_data);
	
	std::cout << std::format(fmt, analyze.get_System_STR(m_system));

	if (!(std::cin >> choose))
	{
		Reset_cin();
		std::cout << "Invalid input system is set\n\a";
		choose = 'n';
	}



	if(std::tolower(choose ) == 'y')
	{
		//	receive new system
		short choose;

		std::cout << "1 - AASHTO\n2 - ASTM \n3 - BSCS \n4 - MIT\nChoose : ";
		std::cin >> choose;
		
		switch (choose) {
		case 1: m_system = System::AASHTO;	break;
		case 2:	m_system = System::ASTM;	break;
		case 3:	m_system = System::BSCS;	break;
		case 4:	m_system = System::MIT;		break;
		default: std::cout << "Invalid selection.Keeping detected system.\n\a";
		}//	end switch
	}//	end if

}//	end function

void Insert::InsertData() {
	
	if (!Check_DIR())
		return;
	
	auto files = Explore_Files();

	const auto path = Get_UserFILE(files);
	
	if (!path)
	{
		std::cerr << "Invalid Path\n\a";
		return;
	}

	ReadData(path.value());
	Sort();
	Set_system();
	
}

System Insert::get_system()const noexcept {
	return m_system;
}

std::vector<std::string> Insert::Explore_Files() {

	//	std::vector<std::string> v;
	//	std::filesystem::path p = Data;
	//	
	//	std::filesystem::directory_iterator start(p);
	//	std::filesystem::directory_iterator end;
	//	
	//	std::transform(start, end, std::back_inserter(v), [](const std::filesystem::directory_entry& entry) {
	//		return entry.path().string();
	//		});
	//	
	//	return v;

	std::vector<std::string> result;

	if (!std::filesystem::exists(Data))
		return result;

	for (auto& entry : std::filesystem::directory_iterator(Data))
		if (entry.is_regular_file())
			result.emplace_back(entry.path().string());

	return result;

}

std::shared_ptr<SieveVector> Insert::return_pointer() {
	return std::make_shared<SieveVector>(m_data);
}

void Insert::ReadData(const std::string_view &filename) {
	std::ifstream in;

	in.open(filename);

	if (!in.is_open())
	{
		std::cerr << "Can not open file '" << filename << "'\n\a";
		return;
	}
	std::string line;

	while (std::getline(in, line))
	{
		std::replace(line.begin(), line.end(), ',', ' ');
		std::stringstream ss(line);
		int No;
		double Size;
		double Passing;
		if (!(ss >> No >> Size >> Passing))
			continue;
		m_data.push_back(Sieve(No, Size, Passing));
	}

	in.close();

	//	data is in m_data

}

std::optional<std::string> Insert::Get_UserFILE(const std::vector<std::string>& files) {
	if (files.empty())
		return std::nullopt;

	for (auto i = 0; i < files.size(); i++)
		std::cout << i + 1 << " - " << files.at(i) << '\n';

	std::cout << "Select file or input your file path : ";
	
	std::string path;
	std::cin.ignore();
	std::getline(std::cin, path);


	if (path.find('\\') != std::string::npos || 
		path.find('/') != std::string::npos || 
		path.find('.') != std::string::npos)
		return path;

	try {
		const auto choice = std::stoi(path);

		if (choice > 0 && choice < files.size())
			return files[choice - 1];

	}
	catch (...) {
		return std::nullopt;
	}

}
