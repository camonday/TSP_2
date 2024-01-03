#include "Reprezentacja.h"
#include <iostream>
#include <fstream>
#include <string>

void Reprezentacja::display() { //wyswietlanie listy od przodu i tylu
   
    std::cout << "\nwartosci\n";
    for (int i = 0; i < size; i++) {
        std::cout << "\t| " << i;
    }
    std::cout << "\t|\n";
    for (int i = 0; i < size; i++) {
        std::cout << "\t ======" ;
    }

    for (int i = 0; i < size; i++) {
        std::cout <<"\n     "<<i<<"\t|";
        for (int j = 0; j < size; j++) {
            std::cout << dane[i][j]<<"\t|";
        }
    }
}

void Reprezentacja::generate(int size) {
    clear(); //czyscimy liste
    this->size = size;
    for (int i = 0; i < size; i++) {
        std::vector<int> wagi;
        for (int j = 0; j < size; j++) {
            wagi.push_back(rand());
        }
        dane.push_back(wagi);
    }
}

int Reprezentacja::loadFromFile(const std::string& FileName) {
    clear(); //czyscimy liste
    std::fstream file;
    file.open(FileName);
    int temp;
    std::string linijka;

    //file >> size; //pierwsza liczba w pliku to rozmiar listy
    getline(file, linijka); //NAME
    getline(file, linijka); //TYPE
    getline(file, linijka);//COMMENT
    getline(file, linijka);//SIZE
    size = 0;
    for (char c : linijka) {
        if (isdigit(c)) {
            size *= 10;
            size += (c - '0');
        }
    }
    getline(file, linijka);//EDGE TYPE
    getline(file, linijka); //FORMAT
    getline(file, linijka); //SECTION

    for (int i = 0; i < size; i++) {
        std::vector<int> wagi;
        for (int j = 0; j < size; j++) {
            file >> temp;
            wagi.push_back(temp);
        }
        dane.push_back(wagi);
    }

    file.close();
    return 0;
}

void Reprezentacja::clear() {
    for (int i = size-1; i >=0; i--) {
        dane[i].clear();
    }
    dane.clear();
}

int Reprezentacja::findDistance(int A, int B)
{
    return dane[A][B];
}

int Reprezentacja::getSize()
{
    return size;
}

int Reprezentacja::getMinDistance(int A)
{
    int min = dane[A][0];
    if (min < 0) {
        min = dane[A][1];
    }
    for (int i = 1; i < size; i++)
    {   
        if (min > dane[A][i] && dane[A][i]>=0) {
            min = dane[A][i];
        }
    }
    return min;
}

