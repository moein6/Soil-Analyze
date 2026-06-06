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
        }// end switch
        
        std::system("pause");
    }// end while
    return 0;
}
