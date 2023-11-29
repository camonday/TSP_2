#pragma once
#include "Algorytm.h"
class SimAnneling :
    public Algorytm
{
    std::list <int> rozwiazanie_X;
    int ocena_X;
    float T; //wspolczynnik T
    int L; 
    enum SchematSchladzania
    {
        liniowy = 1,
        geometryczny = 2,
        logarytmiczny = 3
    } mojSchemat;
    float sigma;


    // zwraca ocene rozwiazania pamietnego jako biezace rozwiazanie danego obiektu
    int funkcjaOceny(std::list <int> rozwiazanie);
    // ustawienie poczatkowej temperatury, poczatkowego rozwiazania
    void poczatek();
    float rozkladBoltzmana(int ocenaSasiada);
    // sprawdzamy czy mamy uwzglednic gorsze miasto na podstawie prawdopodobienstwa i zmiennej losowej
    bool czyUwzglednic(int ocena);
    // przeprowadza schladzanie
    void schladzanie();

public:
    int ZnajdzNajlepsze(Reprezentacja* przekazaneMiasta) override;
private:
    bool czyWarunekKoncowy();
    // podaje sasiada od obecnego rozwiazania
    std::list <int> znajdzSasiada();
};

