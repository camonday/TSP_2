#include "SimAnneling.h"
#include <iostream>
#include <fstream>
#include <algorithm>



int SimAnneling::ZnajdzNajlepsze(Reprezentacja* przekazaneMiasta)
{
    miasta = przekazaneMiasta;
    poczatek();
    int ocenaSasiada;
    std::vector <int> sasiad;
    int ktora_Epoka = 0;
    do {
        ktora_Epoka++;
        for (int iterator = 0; iterator <= L; iterator++) {
            sasiad = znajdzSasiada(rozwiazanie_X);
            ocenaSasiada = funkcjaOceny(sasiad);
            if (ocenaSasiada<ocena_X || czyUwzglednic(ocenaSasiada)) { // if 1st operator is true, 2nd is not evaluated
                rozwiazanie_X = sasiad;
                ocena_X = ocenaSasiada;
                if (ocena_X < waga_UB) {
                    sciezka_UB = rozwiazanie_X;
                    waga_UB = ocena_X;

                   

                    timeEnd = std::chrono::steady_clock::now();
                    time_ofBest = (std::chrono::duration_cast<std::chrono::duration<int>>(timeEnd - timeStart)).count();
                    
                     wynik_co_epoke << time_ofBest << " " << ocena_X << "\n";
                    //std::cout << "\nczas najlepszego rozwiazania = " << time_ofBest;
                    if (waga_UB < ocena_global) {
                        global_best = sciezka_UB;
                        ocena_global = waga_UB;
                        toFile(pliczek);
                        std::cout << "\n nowy globalnie najlepszy:" << ocena_global << "\n";
                    }
                }
            }
        }
        schladzanie();
        timeEnd = std::chrono::steady_clock::now();
        ktora_Epoka = (std::chrono::duration_cast<std::chrono::duration<int>>(timeEnd - timeStart)).count();

        wynik_co_epoke << ktora_Epoka << " " << ocena_X <<"\n";
    } while (!(czyWarunekKoncowy()));

    std::cout << "\n\nTemperatura koncowa Tk = "<<T;
    std::cout << "\nwyrazenie exp(-1/Tk) = "<<rozkladBoltzmana(ocena_X-1);
    std::cout << "\nczas najlepszego rozwiazania = " << time_ofBest;
    std::cout << "\nwynik najlepszego rozwiazania = " << waga_UB<<"\n\n";

    wynik_co_epoke.close();
    return 0;
}

void SimAnneling::setAlpha(float newAlpha)
{
    if (newAlpha > 0 && newAlpha < 1) {
        alpha = newAlpha;
    }
    else {
        std::cout << "\nwspolczynnik poza zakresem (0,1). Ustawiam alpha = 0.5";
        alpha = 0.5;
    }
}

void SimAnneling::setTime(int newTime)
{
    time_sec = newTime;
}

// ustawienie poczatkowej temperatury, poczatkowego rozwiazania
void SimAnneling::poczatek()
{
   
    //ustaw pierwsze rozwiazanie
    ZnajdzUB();
    rozwiazanie_X = sciezka_UB;
    ocena_X = waga_UB;
    if (waga_UB < ocena_global) {
        global_best = sciezka_UB;
        ocena_global = waga_UB;
        toFile(pliczek); 
        std::cout << "\n nowy globalnie najlepszy:" << ocena_global << "\n";
    }
    
    
    showWynik(); // pokazujemy jak wyglada wynik metody zachlannej

    // aby widziec faktycznie znaleziony wynik, ustawiamy losowy wynik jako najlepszy
    // nie ustawiamy go jako rozwiazanie_x poniewaz rozwiazanie poczatkowe ma byc metoda zachlanna
    std::random_shuffle(sciezka_UB.begin(), sciezka_UB.end());
    waga_UB = funkcjaOceny(sciezka_UB);
    std::cout << "UB: "<<waga_UB<<"\n";

    
    int n = miasta->getSize();  
    L = (n * n - n) / 2; //wielkość sasiedztwa
    L /= diverseL; //proporcja do wielkosci sasiedztwa


    //temperature poczatkowa

    //T0 = 123;  //tsp_20_1.txt
    //T0 = 1632; //ft70.atsp
    //T0 = 490;  //ftv55.atsp
    T0 = 513;  //ftv170.atsp
    //T0 = 66;   //rbg358.atsp

    T = T0;


    //ustaw ziarno losowosci
    std::srand(std::time(nullptr));

    wynik_co_epoke.open(skoki);

    //kryterium stopu:
    timeStart = std::chrono::steady_clock::now();
}


// zwraca ocene podanego rozwiazania
// rozwiazanie jest kompletne to znaczy zapętla się
int SimAnneling::funkcjaOceny(std::vector <int> rozwiazanie)
{
    int ocena=0;
    int poprzedni=0;
    for (auto const& miasto : rozwiazanie){
        if(miasto != rozwiazanie.front())
            ocena += miasta->findDistance(miasto, poprzedni);
        poprzedni = miasto;
    }
    return ocena;
}


// przeprowadza schladzanie
void SimAnneling::schladzanie()
{
    T = T * alpha;
}



float SimAnneling::rozkladBoltzmana(int ocenaSasiada)
{
    float p;
    p = ocenaSasiada - ocena_X - 0.0;
    p *= -1.0;
    p = p / T;
    p = exp(p);
    return p;
}




// liczymy prawdop i sprawdzamy czy mamy uwzglednic gorsze miasto
bool SimAnneling::czyUwzglednic(int ocena)
{
    float p = rozkladBoltzmana(ocena);
    float s = (std::rand() % 101) / 100.0;
    if (p >= s) return true;

    return false;
}


bool SimAnneling::czyWarunekKoncowy()
{
    timeEnd = std::chrono::steady_clock::now();

    if ((std::chrono::duration_cast<std::chrono::duration<int>>(timeEnd - timeStart)).count() > time_sec) {
        return true;
    }
    return false;
}


// podaje sasiada od obecnego rozwiazania
// zwracany sąsiad jest kompletny to znaczy ścieżka zapętla się
std::vector<int> SimAnneling::znajdzSasiada(std::vector<int> lokalny)
{
    std::vector<int> sasiad = lokalny;
    int n = miasta->getSize();

    // Wylosuj dwie pozycje, takie ze i<j, i>0, j<n
    int i = std::rand() % (n - 2) + 1; // 1 <= i < n-1
    int j = std::rand() % (n - i - 1) + i + 1; // i+1 <= j < n
    // odwroc sekwencje miedzy pozycjami i i j
     //std::reverse(sasiad.begin() + i, sasiad.begin() + j + 1);
    std::swap(sasiad[i], sasiad[j]);

    return sasiad;
}



void SimAnneling::toFile(std::string fileName)
{
    std::ofstream file;
    file.open(fileName);
    file << miasta->getSize()<<"\n";
    for (auto const& miasto : sciezka_UB) {
        file << miasto << "\n";
    }
    file.close();
}


int SimAnneling::ocenaPliku(std::string fileName)
{
    std::ifstream file;
    file.open(fileName);
    int size, temp;
    std::vector <int> sciezka;

    file >> size;
    if (size == miasta->getSize()) {
        std::cout << "\n\nRozwiazanie odczytane z pliku: ";
        for (int i = 0; i <= size; i++) //i<= bo dlugosc sciezki to ilosc miast +1 -> jedno miasto sie powtarza 
        {
            file >> temp;
            sciezka.push_back(temp);
            std::cout << temp << " ";
        }
        std::cout << "\nOcena rozwiazania z pliku: " << funkcjaOceny(sciezka);
        file.close();
        return funkcjaOceny(sciezka);
    }
    else {
        std::cout << "bledny rozmiar danych";
    }
    file.close();
    return 0;
}

int SimAnneling::roznicaCelu()
{   
    std::vector <int> pierwszy;
    std::vector <int> drugi;
    //1. losuj rozwiazanie
    for (int i = 0; i < miasta->getSize(); ++i)
    {
        pierwszy.push_back(i);
    }
    std::random_shuffle(pierwszy.begin(), pierwszy.end());
    //2. znajdz sasiada
    drugi = znajdzSasiada(pierwszy);
    //3. zwroc roznice ich celow
    int delta = funkcjaOceny(pierwszy) - funkcjaOceny(drugi);
    if (delta < 0) delta *= -1;
    return delta;
}

void SimAnneling::setDiverseL(int k)
{

    diverseL = k;
}

int SimAnneling::getNajlepsze()
{
    return waga_UB;
}

int SimAnneling::getTimeBest()
{
    return time_ofBest;
}

void SimAnneling::setSkoki(std::string fileName)
{
    skoki = fileName;
}

