#pragma once

#include <map>
#include <array>
#include <vector>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <tuple>

constexpr auto null = "NULL";
const size_t sieveCount = 9;

enum class System {
	NUN,
	ASHTO,
	MIT,
	ASTM,
	BSCS,
	ALL
};

namespace Sieve {
	struct NO {
		static constexpr std::array<int, sieveCount> ASTM = { 4,6,8,10,20,40,60,100,200 };
		static constexpr std::array<int, sieveCount> BSCS = { 1,2,3,4,5,6,7,8,9 };
	};


	struct System {
		static constexpr std::array<double, sieveCount> ASTM = { 4.75, 3.35, 2.00, 0.85, 0.425, 0.25, 0.150 , 0.075 , 0.0 };
		static constexpr std::array<double, sieveCount> BSCS = { 5.00, 2.36, 1.18, 0.60, 0.300, 0.150, 0.075, 0.063, 0.0 };
	};

}//	end namespace


class Soil
{

private:

	System m_System;
	double m_D60, m_D30, m_D10, m_CU, m_CC , m_LL , m_PI , M_PL;
	std::map<double, double> m_Data;
	std::string m_ASTM_Name, m_BSCS_Name, m_MIT_Name, m_ASHTO_Name;

public:
	Soil(const System = System::NUN);
	Soil(const std::map<double, double>, const System = System::ASTM);


	void Name_ASTM();
	void Name_MIT();
	void Name_AASHTO();
	void Name_BSCS();
	
	void My_ASTM_Naming();

	//	csv & txt
	void InsertData(std::string = null);

	//	Handy adding
	void Add_Data();

	void Classify();

	void SaveFile_CSV();

	void Print_Data();
private:

	void Set_System(const System );

	void Calculate_CU();
	void Calculate_CC();
	void Calculate_CuCC();

	std::string get_System_STR()const noexcept;

	bool ValidateData(const Soil*)const;
	bool ValidateData(const std::map<double, double>*) const;

	void Copy(const std::map<double, double>*);

	template <typename SieveType>
	auto get_Sieve() -> std::array<double, sieveCount>;
};


