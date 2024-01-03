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
int opcja_int;
float opcja_float;
string fileName;

int main(int argc, char* argv[])
{
    std::ofstream file;

    char option;
    SimAnneling wyzarzanie;
    do {
        std::cout << std::endl;
        std::cout << "==== MENU GLOWNE ===" << std::endl;
        std::cout << "1. Wczytaj z pliku" << std::endl;
        std::cout << "2. Wyswietl" << std::endl;
        std::cout << "--------------------" << std::endl;
        std::cout << "   PODANIE PARAMETROW:" << std::endl;
        std::cout << "3. Podaj kryterium stopu - czas wykonania w sekundach" << std::endl;
        std::cout << "4. Podaj wspolczynnik alpha zmiany temperatury" << std::endl;
        std::cout << "--------------------" << std::endl;
        std::cout << "5. Znajdz droge - SW" << std::endl;
        std::cout << "6. Zapisz wynik do pliku .txt" << std::endl;
        std::cout << "7. Wczytaj sciezke i znajdz koszt" << std::endl;
        std::cout << "--------------------" << std::endl;
        std::cout << "8. znajdz t0" << std::endl;
        std::cout << "9. diverseL" << std::endl;
        std::cout << "0.Wyjscie" << std::endl;
        std::cout << "Podaj opcje:";
        option = _getche();
        std::cout << std::endl;

        switch (option) {
        case '1':   // 1. Wczytaj z pliku
            std::cout << " Podaj nazwe zbioru:";
            std::cin >> fileName;
            miasta.loadFromFile(fileName);

        case '2':   // 2. Wyświetl
            miasta.display();
            break;

        case '3':   // 3. Kryterium stopu
            std::cout << "Podaj czas trwania algorytmu (sekundy): ";
            std::cin >> opcja_int;
            wyzarzanie.setTime(opcja_int);
            break;

        case '4':   // 4. Parametr alpha
            std::cout << "Podaj parametr alpha w formacie 0.x : ";
            std::cin >> opcja_float;
            wyzarzanie.setAlpha(opcja_float);
            break;

        case 'a':
            timeStart = steady_clock::now();
            wyzarzanie.ZnajdzNajlepsze(&miasta);
            timeEnd = steady_clock::now();

            timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
            std::cout << "\n czas: " << timeTemp.count() << " sekund";
            wyzarzanie.showWynik();
            break;
        case '5': // 5. Znajdź sciezkę
            wyzarzanie.setTime(25);
            miasta.loadFromFile("ftv170.atsp");

            wyzarzanie.setAlpha(0.75);
            file.open("wlochy_sredni1.txt");
            
            std::cout << "\n";

            for (int i = 0; i < 10; i++) {
                wyzarzanie.setSkoki("alpha1_" + std::to_string(i) + "a.txt");
               // timeStart = steady_clock::now();
                wyzarzanie.ZnajdzNajlepsze(&miasta);
               // timeEnd = steady_clock::now();

               // timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
               // std::cout << "\n czas: " << timeTemp.count() << " sekund";
               // wyzarzanie.showWynik();
                file << wyzarzanie.getNajlepsze() << " " << wyzarzanie.getTimeBest() << "\n";
            }
            file.close();

           

            break;

        case '6':   // 6. zapisz do pliku
            std::cout << "Podaj nazwe pliku: ";
            std::cin >> fileName;
            wyzarzanie.toFile(fileName);
            break;
        case '7':   // 7. Wczytaj sciezke znajdz koszt
            std::cout << "Podaj nazwe pliku: ";
            std::cin >> fileName;
            wyzarzanie.ocenaPliku(fileName);
            break;
        case '8':   // 8. znajdz t0
            wyzarzanie.setMiasta(&miasta);
            //std::ofstream file;
           // file.open("temperatura.txt");
     
            for (int i = 0; i <= 10000; i++) {
                //file << wyzarzanie.roznicaCelu() << "\n";
            }

            //file.close();

            break;
        case '9':   // 4. Parametr alpha
            std::cout << "Podaj parametr k/L : ";
            std::cin >> opcja_int;
            wyzarzanie.setDiverseL(opcja_int);
            break;
        }
    } while (option != '0');


    return 0;
}
