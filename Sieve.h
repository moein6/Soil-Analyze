#pragma once
#include <array>
#include <cmath>
#include <format>
#include <vector>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <algorithm>


//  cls : clear screen 
#define cls std::system("cls");


enum class System {
    NUN,
    AASHTO,
    MIT,
    ASTM,
    BSCS,
    ALL
};

class Sieve {
public:
    int No;
    double Size;
    double Passing;

    Sieve();
    Sieve(const int&, const double&, const double&);

    std::string get_System_STR(const System& sys) const noexcept;

    void Reset_cin()const;
};

using SieveVector = std::vector<Sieve>;

const short sieveCount = 9;

struct NO {
    static constexpr std::array<int, sieveCount> ASTM = { 4,8,10,20,40,60,100,200,0 };
    static constexpr std::array<int, sieveCount> BSCS = { 4,8,16,30,40,50,70,100,200 };
};

struct Size {
    static constexpr std::array<double, sieveCount> ASTM = { 4.75, 2.36, 2.00, 0.85, 0.425, 0.25, 0.150 , 0.075 , 0.0 };
    static constexpr std::array<double, sieveCount> BSCS = { 5.00, 2.36, 1.18, 0.60, 0.300, 0.150, 0.075, 0.063, 0.0 };
};


const SieveVector ASTM = {
    {4,4.75,-1},
    {8,2.36,-1},
    {10,2.00,-1},
    {20,0.85,-1},
    {40,0.425,-1},
    {60,0.25,-1},
    {100,0.150,-1},
    {200,0.075,-1},
    {0,0.0,-1}
};

const SieveVector BSCS = {
    {1,5.00,-1},
    {2,2.36,-1},
    {3,1.18,-1},
    {4,0.60,-1},
    {5,0.30,-1},
    {6,0.15,-1},
    {7,0.075,-1},
    {8,0.063,-1},
    {9,0.0,-1},
};

const SieveVector AASHTO = ASTM;

const SieveVector MIT = {
    {1,5.00,-1},
    {2,2.36,-1},
    {3,1.18,-1},
    {4,0.60,-1},
    {5,0.30,-1},
    {6,0.15,-1},
    {7,0.075,-1},
    {8,0.063,-1},
    {9,0.0,-1},
};