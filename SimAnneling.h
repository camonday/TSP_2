#pragma once
#include "Algorytm.h"
#include <chrono>
#include <windows.h>
#include <iostream>
#include <fstream>
#include<conio.h>

class SimAnneling :
    public Algorytm
{
    std::vector <int> rozwiazanie_X, global_best;
    int ocena_X, ocena_global=30000;
    float T0;
    float T; //wspolczynnik T
    int L=5; 
    int diverseL=1;
    float alpha=0.5;
    int time_sec;
    int time_ofBest;
    std::chrono::steady_clock::time_point timeStart, timeEnd;
    std::string pliczek = "bestof_rgb.txt"; //!!! - pamietac o zmianie nazwy pliku tekstowego
    std::string skoki = "wtoreek.txt";
    std::ofstream wynik_co_epoke;


    // zwraca ocene rozwiazania pamietnego jako biezace rozwiazanie danego obiektu
    int funkcjaOceny(std::vector <int> rozwiazanie);
    // ustawienie poczatkowej temperatury, poczatkowego rozwiazania
    void poczatek();
    float rozkladBoltzmana(int ocenaSasiada);
    // sprawdzamy czy mamy uwzglednic gorsze miasto na podstawie prawdopodobienstwa i zmiennej losowej
    bool czyUwzglednic(int ocena);
    // przeprowadza schladzanie
    void schladzanie();
    bool czyWarunekKoncowy();
    // podaje sasiada od obecnego rozwiazania
    std::vector <int> znajdzSasiada(std::vector <int> lokalny);
  

public:
    int ZnajdzNajlepsze(Reprezentacja* przekazaneMiasta) override;
    void setAlpha(float newAlpha);
    void setTime(int newTime);

    void toFile(std::string fileName);
    int ocenaPliku(std::string fileName);
    int roznicaCelu();
    void setDiverseL(int k);
    int getNajlepsze();
    int getTimeBest();
    void setSkoki(std::string fileName);
};

