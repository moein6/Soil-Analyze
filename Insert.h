#pragma once


#include "Analyze.h"

#include <string>
#include <fstream>
#include <optional>
#include <filesystem>
#include <ranges>


const std::filesystem::path Root = "D:\\Soil DataBase";
const std::filesystem::path History = Root.string() + "\\History";
const std::filesystem::path Data = Root.string() + "\\new";


class Insert
{
	friend class Analyze;

private:

	SieveVector m_data;
	System m_system;
	
public:
	Insert();

	void InsertData();

	System get_system()const noexcept;	
	
	std::shared_ptr<SieveVector> return_pointer();
	

private:

	void Sort();
	void Print();			
	bool Check_DIR();
	void Set_system();
	void ReadData(const std::string_view&);
	void Reset_cin()const;
	std::vector<std::string> Explore_Files();

	std::optional<std::string> Get_UserFILE(const std::vector<std::string>&);
};