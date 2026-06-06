#pragma once

#include <iomanip>
#include <Windows.h>
#include <source_location>
#include <string>


#if __has_include(<iostream>)
#include <iostream>
#endif // __has_include(<iostream>)

#define	PAUSE  std::cin.get();std::cin.get();
#define CLEAN std::system("cls")
#define Stop PAUSE


template<typename Ty = unsigned short> Ty Get(const char* Input) {
	//	input Ty data controller
Try:

	Ty output{};
	
	std::cout << Input << " : ";
	std::cin >> output;

	if (std::cin.fail()) {
		std::cin.clear();
		std::cin.setf(std::cin.goodbit);

		std::cin.get();
		goto Try;
	}


	return output;
}
template<typename Ty = unsigned short> Ty Get(const char* Options, const unsigned short Limite) {
	//	menu controller
Try:

	Ty output;

	do
	{
		std::cout << Options << "\nChoose : ";
		std::cin >> output;
		
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.setf(std::cin.goodbit);
		
			std::cin.get();
			goto Try;
		}

	} while (output > Limite or output < 0);


	return output;
}

enum PositionX : SHORT
{
	Begin = 0,
	Middle = 50,
	End = 100
};

enum PositionY : SHORT
{
	Top = 0,
	middle = 10,
	Buttom = 30
};

class Screen
{


public:

	//	show Text
	template <typename Ty>	friend Screen& operator << (Screen& screen, const Ty text) {
		std::cout << text;
		return screen;
	}

	//	show Message 
	template <typename Ty>	friend Screen& operator == (Screen& screen, const Ty text) {
		std::cout << "[MASSAGE] : " << text;
		return screen;
	}

	//	show Error
	template <typename Ty>	friend Screen& operator || (Screen& screen, const Ty text) {
		std::cerr << "[ERROR] : " << text << '\a';
		return screen;
	}

	//	set the output position using enum
	void gotoxy(PositionX x, PositionY y) {
		//	select position
		HANDLE hndl = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD c = { x , y };
		SetConsoleCursorPosition(hndl, c);
	}
	
	//	set the output position using manual numbers.
	void gotoxy(SHORT x, SHORT y) {
		//	enter position
		HANDLE hndl = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD c = { x , y };
		SetConsoleCursorPosition(hndl, c);
	}

#if __cplusplus > 199710L

	//	Show code line and message
	auto Code_Error(const std::string_view Message, const std::source_location Code_Data = std::source_location::current()) {

		*this || Message;
		*this << "\nFile : " << Code_Data.file_name() << '\n';
		*this << "function :" << Code_Data.function_name() << '\n';
		*this << "Line : " << Code_Data.line() << '\n';

	}

#endif // 0


};

/*
	Do we need some code that creats a table at diffrent sizs just like the soduko generator?


*/
