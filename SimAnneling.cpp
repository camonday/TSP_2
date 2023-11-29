#include "SimAnneling.h"



int SimAnneling::ZnajdzNajlepsze(Reprezentacja* przekazaneMiasta)
{
    // TODO: Add your implementation code here.
    poczatek();
    int ocenaSasiada;
    std::list <int> sasiad;
    do {
        for (int iterator = 0; iterator <= L; iterator++) {
            sasiad = znajdzSasiada();
            ocenaSasiada = funkcjaOceny(sasiad);
            if (ocenaSasiada<ocena_X || czyUwzglednic(ocenaSasiada)) { // if 1st operator is true, 2nd is not evaluated
                rozwiazanie_X = sasiad;
            }
        }
        schladzanie();
    } while (!(czyWarunekKoncowy()));

    return 0;
}

// ustawienie poczatkowej temperatury, poczatkowego rozwiazania
void SimAnneling::poczatek()
{
    //ustaw T
    T = 10000;
    //ustaw L
    L = 10;
    //ustaw parametry schladzania
    mojSchemat = liniowy;
    sigma = 0.5;
    //ustaw pierwsze rozwiazanie
    rozwiazanie_X.clear();
    for (int i=0; i<=miasta->getSize(); i++){
        rozwiazanie_X.push_back(i);
    }
    ocena_X = funkcjaOceny(rozwiazanie_X);
    //kryterium stopu:
    //TODO: zaimplementowac kryterium stopu;
}


// zwraca ocene podanego rozwiazania
int SimAnneling::funkcjaOceny(std::list <int> rozwiazanie)
{
    int ocena=0;
    int poprzedni;
    for (auto const& miasto : rozwiazanie){
        if(miasto != rozwiazanie.front())
            ocena += miasta->findDistance(miasto, poprzedni);
        poprzedni = miasto;
    }
    ocena += miasta->findDistance(poprzedni, rozwiazanie.front());
    return ocena;
}



float SimAnneling::rozkladBoltzmana(int ocenaSasiada)
{
    // TODO: Add your implementation code here.
    return 0.0f;
}


// przeprowadza schladzanie
void SimAnneling::schladzanie()
{
    float zmiana;
    switch (mojSchemat)
    {
    case SimAnneling::liniowy:
        //zmiana = x
        break;
    case SimAnneling::geometryczny:
        break;
    case SimAnneling::logarytmiczny:
        break;
    default:
        break;
    }
    T -= zmiana;
}


// liczymy prawdop i sprawdzamy czy mamy uwzglednic gorsze miasto
bool SimAnneling::czyUwzglednic(int ocena)
{
    float p = rozkladBoltzmana(ocena);
    float s = 0; //rand 0-1
    if (p >= s) return true;

    return false;
}


bool SimAnneling::czyWarunekKoncowy()
{
    // TODO: Add your implementation code here.
    return false;
}


// podaje sasiada od obecnego rozwiazania
std::list <int> SimAnneling::znajdzSasiada()
{
    // TODO: Add your implementation code here.
    return std::list <int>();
}
