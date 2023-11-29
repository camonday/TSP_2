#include "Algorytm.h"
#include <iostream>

int Algorytm::ZnajdzNajlepsze(Reprezentacja* przekazaneMiasta)
{
    std::cout << "nie wybrales algorytmu";
    return 0;
}

void Algorytm::showWynik()
{
	std::cout << "\nwaga: " << waga_UB << "\nścieżka: ";
	for (int& przystanek : sciezka_UB) {
		std::cout << przystanek << " ";
	}
}

void Algorytm::ZnajdzUB()
{
	int startoweMiasto = 0;
	int badany = startoweMiasto;
	int waga_pojedyncza = 0;
	int nastepny;

	// lista nieodwiedzonych sasiadow
	std::list<int> nieodwiedzeni;
	for (int i = 1; i < miasta->getSize(); i++)
	{
		nieodwiedzeni.push_back(i);
	}
	sciezka_UB.push_back(startoweMiasto);
	while (nieodwiedzeni.size() > 0) {
		waga_pojedyncza = RAND_MAX;
		for (int& sasiad : nieodwiedzeni) {
			//sprawdz kazdego nieodwiedzonego jeszcze sasiada i wybierz najlepszego
			if (waga_pojedyncza > miasta->findDistance(badany, sasiad)) {
				waga_pojedyncza = miasta->findDistance(badany, sasiad);
				nastepny = sasiad;
			}
		}

		//usun najlepszego sasiada z nieodwiedzonych
		nieodwiedzeni.remove(nastepny);
		badany = nastepny;

		//uaktualnij dane UB
		waga_UB += waga_pojedyncza;
		sciezka_UB.push_back(nastepny);
	}
	// dodaj polaczenie wracajae do miasta startowego
	waga_UB += miasta->findDistance(badany, startoweMiasto);
	sciezka_UB.push_back(startoweMiasto);

}
