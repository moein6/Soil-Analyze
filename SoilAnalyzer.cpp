#include "Soil.h"
#include <Windows.h>

#include <string>
#include <math.h>

short Menu(const char* options, const short Limit) {
    short choose;

    do{
        std::cout << options << "\n-> ";
        std::cin >> choose;
    } while (choose < 0 or choose > Limit);

    std::system("Cls");
    return choose;
}

int main()
{
    std::cout.precision(9);
    
    const double a = 8742.25;

    for (double c = 50; c < 200; c += 0.00001) {

        auto res = a * c + 132000000 * ((c - 65) / c) - 924000;

        if (res > 0) {
            std::cout << "c = " << c << ' ' << res << '\n';
            break;
        }

    }




#if 0


    Soil s;
    
    while (true) {
        std::system("cls");
            
        switch (Menu("1 - Insert File (CSV/TXT)\n2 - Input data\n3 - Print\n4 - Save data(CSV)\n5 - clear\n6 - Help\n0 - exit", 6)) {
        case 1: s.Insert_data();         break;
        case 2: s.Input_data();          break;
        case 3: s.Print();          break;
        case 4: s.SaveFile_CSV();   break;
        case 5: s.Clear();          break;
        case 6: s.Help();           break;
        case 0: exit(1);            
        }// wns switch
        
        std::system("pause");
    }// end while
    return 0;

#endif
}


/*
    to save different data in file i should use the date of saving file.

*/