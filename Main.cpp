#include<conio.h>
#include<string>
#include<iostream>
#include <fstream>
#include <chrono>
#include <windows.h>
#include <vector>
#include "Reprezentacja.h"
#include "Algorytm.h"
#include "SimAnneling.h"

double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        std::cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart);

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}
double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart - CounterStart) / PCFreq;
}

using namespace std;
using namespace std::chrono;

struct Comma final : std::numpunct<char>
{
    char do_decimal_point() const override { return ','; }
};

steady_clock::time_point timeStart, timeEnd;
duration<double> timeTemp, timeSum;
Reprezentacja miasta; 
int rozmiar;
string fileName;

int main(int argc, char* argv[])
{

    char option;
    char option2;
    SimAnneling wyzarzanie;
    do {
        std::cout << std::endl;
        std::cout << "==== MENU GLOWNE ===" << std::endl;
        std::cout << "1.Wczytaj z pliku" << std::endl;
        std::cout << "2.Wylosuj" << std::endl;
        std::cout << "3.Wyświetl" << std::endl;
        std::cout << "4.Znajdź drogę" << std::endl;
        std::cout << "--------------------" << std::endl;
       // std::cout << "5.Testy" << std::endl;
        std::cout << "0.Wyjscie" << std::endl;
        std::cout << "Podaj opcje:";
        option = _getche();
        std::cout << std::endl;

        switch (option) {
        case '1':   // 1. Wczytaj z pliku
            std::cout << " Podaj nazwę zbioru:";
            std::cin >> fileName;
            miasta.loadFromFile(fileName);
            miasta.display();
            break;

        case '2':   // 2. Wylosuj
            std::cout << " Podaj ilość miast:";
            std::cin >> rozmiar;
            miasta.generate(rozmiar);

        case '3':   // 3. Wyświetl
            miasta.display();
            break;

        case '4': // 4. Znajdź sciezkę
            //zapytaj jaką metodą
            timeStart = steady_clock::now();
            wyzarzanie.ZnajdzNajlepsze(&miasta);
            timeEnd = steady_clock::now();

            timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
            std::cout << "\n czas: " << timeTemp.count() << " sekund";
            wyzarzanie.showWynik();

            break;
        }
    } while (option != '0');


    return 0;
}
