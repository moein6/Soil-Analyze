#pragma once
#include "Input.h"


Input::Input()
    : m_system(System::NUN)
{
    m_data.reserve(32);
}

bool Input::SelectSystem()
{
    std::cout
        << "Choose sieve system:\n"
        << "1 - AASHTO\n"
        << "2 - ASTM \n"
        << "3 - BSCS \n"
        << "4 - MIT\n"
        << "> ";

    int choose{};
    if (!(std::cin >> choose)) {
        Reset_cin();
        std::cerr << "Invalid input.\n";
        return false;
    }

    switch (choose) {
    case 1: m_system = System::AASHTO; break;
    case 2: m_system = System::ASTM;   break;
    case 3: m_system = System::BSCS;   break;
    case 4: m_system = System::MIT;    break;
    default:
        std::cerr << "Unknown option. Try again.\n";
        return false;
    }

    return true;
}

bool Input::CollectUserInput()
{
    if (m_data.empty())
        return false;

    std::cout << std::format("{:^8}| {:^8}|{:^8}\n", "NO", "SIZE", "PASS");
    std::cout << std::string(30, '-') << "\n";

    for (auto& s : m_data)
        if (!ReadValueFor(s))
            return false;
    
    return true;
}

System Input::get_system() const noexcept
{
    return m_system;
}

void Input::Reset_cin()const {
    Sieve sieve;

    sieve.Reset_cin();
}

bool Input::ReadValueFor(Sieve& sieve)
{
    //  User can try 3 times


    for (auto i = 0; i < 3; i++) {
        float passing{};

        std::cout << std::format("{:^8}|{:^8}|{:^4}", sieve.No, sieve.Size, " ");

        if (!(std::cin >> passing)) {
            Reset_cin();
            std::cerr << "Invalid number, try again.\n";
            continue;
        }

        if (passing < 0 || passing > 100) {
            std::cerr << "Value must be between 0 and 100, try again.\n";
            continue;
        }

        sieve.Passing = passing;
        return true;
    }// end for

    return false;

}


bool Input::Run()
{
    if (!SelectSystem())
        return false;

    auto initial = InitializeData();
    if (!initial) {
        std::cerr << "Failed to initialize data\n";
        return false;
    }

    m_data = *initial;

    if (!CollectUserInput()) {
        std::cerr << "Invalid data detected.\n";
        return false;
    }

    return true;
}

std::optional<SieveVector> Input::InitializeData()
{
    switch (m_system) {
    case System::AASHTO: return AASHTO;
    case System::ASTM:   return ASTM;
    case System::MIT:    return MIT;
    case System::BSCS:   return BSCS;
    case System::ALL:    return std::nullopt;
    case System::NUN:
        std::cerr << "No system selected.\n";
        return std::nullopt;
    }
    return std::nullopt;
}

std::shared_ptr<SieveVector> Input::Return_Pointer() const
{
    return std::make_shared<SieveVector>(m_data);
}
