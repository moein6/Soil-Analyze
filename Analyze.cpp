#include "Analyze.h"

Analyze::Analyze() {
	Detected = false;
	m_pASTMno = m_pBSCSno = m_pMITno = m_pAASHTOno = m_pASTMsize = m_pBSCSsize = m_pAASHTOsize = m_pMITsize = 0.0;

}

void Analyze::Print(const System& sys) {
	
	std::cout.precision(4);

	const auto ave_astm = (m_pASTMno + m_pASTMsize) / 2;
	const auto ave_bscs = (m_pBSCSno + m_pBSCSsize) / 2;

	std::cout << "Detected as : " << Sieve::get_System_STR(sys) << '\n';

	const auto title = "{:<10}|{:^10}|{:^10}|{:^10}\n";
	const auto elem = "{:<10}|{:^10.2f}|{:^10.2f}|{:^10.2f}\n";
	

	if (ave_astm || ave_bscs) {
		std::cout << std::format(title, "SYS", "NO", "SIZE", "AVE") << std::string(40, '-') << '\n';

		if (ave_astm > 0)
			std::cout << std::format(elem, "ASTM", m_pASTMsize, m_pASTMno, ave_astm);

		if (ave_bscs > 0)
			std::cout << std::format(elem, "BSCS", m_pBSCSsize, m_pBSCSno, ave_bscs);

	}
}

bool Analyze::Analyze_ASTM(const SieveVector&vec) {
	
	bool Continue = true;
	bool Confirmed = false;
	float countNo = 0, countSize = 0;

	std::vector<int> vec_No, astm_No;
	std::vector<double> vec_size, astm_size;

	for (const auto& [no, size, pas] : ASTM) {
		astm_No.push_back(no);
		astm_size.push_back(size);
	}

	for (const auto& [no, size, pas] : vec) {
		vec_No.push_back(no);
		vec_size.push_back(size);
	}

	//	if numbers are equal
	if (std::equal(astm_No.begin(), astm_No.end(), vec_No.begin(), vec_No.end()))
		if (std::equal(astm_size.begin(), astm_size.end(), vec_size.begin(), vec_size.end()))  //	if sizes are equal
			Confirmed = true;
		
	//	if not equal than lets find sameness 
	if (!Confirmed) {

		for (const auto& no : astm_No)
			if (std::find(vec_No.begin(), vec_No.end(), no) != vec_No.end())
				countNo++;

		for (const auto& size : astm_size)
			if (std::find(vec_size.begin(), vec_size.end(), size) != vec_size.end())
				countSize++;
	}//	end else
		

	m_pASTMno = (countNo / astm_No.size()) * 100;
	m_pASTMsize = (countSize / astm_size.size()) * 100;



#if 0
	//	first way with error in size

	//	0.5 < x < 2.35

	for (const auto& [vec_no, vec_size, vec_pass] : vec) {
		bool foundNo = false;
		if (Continue) {
			for (const auto& [astm_no, astm_size, astm_pass] : m_astm) {


				//	find NO
				if (vec_no == astm_no) {
					count++;

					foundNo = true;

					//	calculate Error percent
					const auto& Err = (std::abs(astm_size - vec_size) / astm_size) * 100;

					//	if error is more than allowed range : stop looping and quit function return false.
					if (Err > 2.40) {

						Confirmed = false;
						Continue = false;
						std::cerr << "This sieve do not match ASTM system : #" << vec_no << " , " << vec_size << "mm\n\a";

						//	return false;
					}//	end if

					break;

				}//	end if
			}//	end for
		}//	end if
		else break;

		if (foundNo == false)
		{
			Confirmed = false;
			Continue = false;
			//	return false;
		}//	end if

	}//	end for

	percent_astm = (count / m_astm.size()) * 100;

#endif

	return Confirmed;
}

bool Analyze::Analyze_BSCS(const SieveVector& vec) {

	bool Continue = true;
	bool Confirmed = false;
	float countNo = 0, countSize = 0;

	std::vector<int> vec_No, bscs_No;
	std::vector<double> vec_size, bscs_size;

	for (const auto& [no, size, pas] : BSCS) {
		bscs_No.push_back(no);
		bscs_size.push_back(size);
	}

	for (const auto& [no, size, pas] : vec) {
		vec_No.push_back(no);
		vec_size.push_back(size);
	}

	//	if numbers are equal
	if (std::equal(bscs_No.begin(), bscs_No.end(), vec_No.begin(), vec_No.end()))
		if (std::equal(bscs_size.begin(), bscs_size.end(), vec_size.begin(), vec_size.end()))  //	if sizes are equal
			Confirmed = true;

	//	if not equal than lets find sameness 
	if (!Confirmed) {

		for (const auto& no : bscs_No)
			if (std::find(vec_No.begin(), vec_No.end(), no) != vec_No.end())
				countNo++;

		for (const auto& size : bscs_size)
			if (std::find(vec_size.begin(), vec_size.end(), size) != vec_size.end())
				countSize++;
	}//	end else


	m_pBSCSno = (countNo / bscs_No.size()) * 100;
	m_pBSCSsize = (countSize / bscs_size.size()) * 100;


#if 0
	//	first way with error in sizes
	//	0.5 < x < 2.35

	for (const auto& [vec_no, vec_size, vec_pass] : vec) {
		bool foundNo = false;
		if (Continue) {
			for (const auto& [bscs_no, bscs_size, bscs_pass] : m_bscs) {


				//	find NO
				if (vec_no == bscs_no) {

					foundNo = true;

					//	calculate Error percent
					const auto& Err = (std::abs(bscs_size - vec_size) / bscs_size) * 100;

					//	if error is more than allowed range : stop looping and quit function return false.
					if (Err > 2.40) {

						Confirmed = false;
						Continue = false;
						std::cerr << "This sieve do not match BSCS system : #" << vec_no << " , " << vec_size << "mm\n\a";

						//	return false;
					}//	end if

					break;

				}//	end if
			}//	end for
			count++;
		}//	end if
		else break;

		if (foundNo == false)
		{
			Confirmed = false;
			Continue = false;
			//	return false;
		}//	end if
	}//	end for

	percent_bscs = (count / m_bscs.size()) * 100;

#endif
	
	return Confirmed;

}

bool Analyze::Analyze_MIT(const SieveVector& vec) {

	bool Continue = true;
	bool Confirmed = false;
	float countNo = 0, countSize = 0;

	std::vector<int> vec_No, T_No;
	std::vector<double> vec_size, T_size;

	for (const auto& [no, size, pas] : MIT) {
		T_No.push_back(no);
		T_size.push_back(size);
	}

	for (const auto& [no, size, pas] : vec) {
		vec_No.push_back(no);
		vec_size.push_back(size);
	}

	//	if numbers are equal
	if (std::equal(T_No.begin(), T_No.end(), vec_No.begin(), vec_No.end()))
		if (std::equal(T_size.begin(), T_size.end(), vec_size.begin(), vec_size.end()))  //	if sizes are equal
			Confirmed = true;

	//	if not equal than lets find sameness 
	if (!Confirmed) {

		for (const auto& no : T_No)
			if (std::find(vec_No.begin(), vec_No.end(), no) != vec_No.end())
				countNo++;

		for (const auto& size : T_size)
			if (std::find(vec_size.begin(), vec_size.end(), size) != vec_size.end())
				countSize++;
	}//	end else


	m_pBSCSno = (countNo / T_No.size()) * 100;
	m_pBSCSsize = (countSize / T_size.size()) * 100;

	return Confirmed;
}

bool Analyze::Analyze_AASHTO(const SieveVector& vec) {
	bool Continue = true;
	bool Confirmed = false;
	float countNo = 0, countSize = 0;

	std::vector<int> vec_No, T_No;
	std::vector<double> vec_size, T_size;

	for (const auto& [no, size, pas] : AASHTO) {
		T_No.push_back(no);
		T_size.push_back(size);
	}

	for (const auto& [no, size, pas] : vec) {
		vec_No.push_back(no);
		vec_size.push_back(size);
	}

	//	if numbers are equal
	if (std::equal(T_No.begin(), T_No.end(), vec_No.begin(), vec_No.end()))
		if (std::equal(T_size.begin(), T_size.end(), vec_size.begin(), vec_size.end()))  //	if sizes are equal
			Confirmed = true;

	//	if not equal than lets find sameness 
	if (!Confirmed) {

		for (const auto& no : T_No)
			if (std::find(vec_No.begin(), vec_No.end(), no) != vec_No.end())
				countNo++;

		for (const auto& size : T_size)
			if (std::find(vec_size.begin(), vec_size.end(), size) != vec_size.end())
				countSize++;
	}//	end else


	m_pBSCSno = (countNo / T_No.size()) * 100;
	m_pBSCSsize = (countSize / T_size.size()) * 100;

	return Confirmed;
}

System Analyze::AnalyzeData(const SieveVector& vec) {
	
	//	try astm
	if (Analyze_ASTM(vec)) {
		//	Print(System::ASTM);
		return System::ASTM;
	}
	//	try bscs
	else if (Analyze_BSCS(vec)) {
		//	Print(System::BSCS);
		return System::BSCS;
	}//	try aashto
	else if (Analyze_AASHTO(vec)) {
		//	Print(System::ASSHTO);
		return System::AASHTO;
	}//	try mit
	else if (Analyze_MIT(vec)) {
		//	Print(System::MIT);
		return System::MIT;
	}


	//	Print(System::NUN);
	return System::NUN;



}//	end function
