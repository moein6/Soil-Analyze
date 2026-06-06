#pragma once


#include "Input.h"
#include "Sieve.h"
#include "Insert.h"
#include "Analyze.h"


class Soil 
{

	friend class Insert;
	friend class Input;
	friend class Analyze;
	friend class Sieve;

private:

	System m_System;

	std::shared_ptr<SieveVector> m_Data;
	
	double m_D60, m_D30, m_D10, m_CU, m_CC, m_LL, m_PI, m_PL;

	std::string m_ASTM_Name, m_BSCS_Name, m_MIT_Name, m_ASHTO_Name;

public:
	Soil();
	
	void Insert_data();
		
	void Input_data();

	void SaveFile_CSV();

	void Print();

	void Clear();

	void Help();

private:

	void Classify();

	void Name_ASTM();
	void Name_MIT();
	void Name_AASHTO();
	void Name_BSCS();
	
	void Proccess();
	void Get_Atterberg();

	void Calculate_CU();
	void Calculate_CC();
	void Calculate_Param();

	void DATA(std::ostream&);
	

	bool GoodToGo()const;
};
