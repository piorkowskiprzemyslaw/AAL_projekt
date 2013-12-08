#ifndef CONTAINERSSWEEP_H
#define CONTAINERSSWEEP_H

#include <iostream>
#include "ContainerSet.h"

/*
 * Klasa realizujaca podejscie do rozwiazania problemu za pomoca omiatania pojemnikow z klockow
 * o kolorach ktore juz nie moga sie w tym pojemniku znajdowac.
 */
template <int(*T)(std::pair<unsigned int, unsigned int> *, unsigned int)>
class ContainersSweep
{
private:
	ContainerSet * containerSet;
	size_t * colorMultiplicity;
	bool checkPreconditions();
	void organizeColor(std::pair<unsigned int, unsigned int> * table, unsigned int colorNo);
public:
	ContainersSweep();
	ContainersSweep(const ContainerSet & containerSet);
	~ContainersSweep();
	void solveProblem();
};

/*
* Domyslny konstruktor.
*/
template <int(*T)(std::pair<unsigned int, unsigned int> *, unsigned int) >
ContainersSweep<T>::ContainersSweep()
{
	containerSet = nullptr;
	colorMultiplicity = nullptr;
}


/*
* Konstrutkor pobierajacy konkretny zbior pojemnikow.
*/
template <int(*T)(std::pair<unsigned int, unsigned int> *, unsigned int) >
ContainersSweep<T>::ContainersSweep(const ContainerSet & containerSet)
{
	this->containerSet = new ContainerSet(containerSet);
	colorMultiplicity = new size_t[this->containerSet->getColorsNumber()];
}

/*
* Destruktor.
*/
template <int(*T)(std::pair<unsigned int, unsigned int> *, unsigned int) >
ContainersSweep<T>::~ContainersSweep()
{
	delete(containerSet);
	delete(colorMultiplicity);
}

/*
* Sprawdz warunki poczatkowe.
*/
template <int(*T)(std::pair<unsigned int, unsigned int> *, unsigned int) >
bool ContainersSweep<T>::checkPreconditions()
{
    unsigned int allColorSum = 0;

	//liczba kolorow musi byc mniejsza badz rowna liczbe dostepnych pojemnikow.
	if (this->containerSet->getColorsNumber() > this->containerSet->size())
	{
		return false;
	}

	//lacznie we wszystkich pojemnikach jest nie wiecej niz n klockow danego koloru.
	//przy okazji wykonuje zliczanie każdego koloru - potrzebne do dalszej czesci algorytmu,
	for (int i = 0; i < containerSet->getColorsNumber(); i++)
	{
		colorMultiplicity[i] = containerSet->colorMultiplicity(i);
		allColorSum += colorMultiplicity[i];
		if (colorMultiplicity[i] > containerSet->size())
		{
			return false;
		}

	}

	//zapewnienie aby wszystkie klocki miescily sie w pojemnikach, oraz zeby dalo
	//sie wykonac ruch.
	if (allColorSum >= containerSet->getCapacitySum())
	{
		return false;
	}

	return true;
}

/*
* Metoda rozwiazujaca problem implementowanym algorytmem.
*/
template <int(*T)(std::pair<unsigned int, unsigned int> *, unsigned int) >
void ContainersSweep<T>::solveProblem()
{
    //talibca z parami <kolor, ilosc wystapien> potrzebna do zrealizowania algorytmu.
    std::pair<unsigned int, unsigned int > * table = new std::pair<unsigned int, unsigned int>[containerSet->getColorsNumber()];
	std::cout << "Solving problem with ContainersSweep" << std::endl;
	std::cout << "Checking preconditions ... ";

	if (!checkPreconditions())
	{
		std::cout << "[FALSE]" << std::endl;
		std::cout << "End of solving problem" << std::endl;
		return;
	}
	std::cout << "[OK]" << std::endl;
	for (int i = 0; i < containerSet->getColorsNumber(); ++i)
	{
        table[i] = std::make_pair(i, colorMultiplicity[i]);
    }

	T(table, containerSet->getColorsNumber());

	for(int i = 0 ; i < containerSet->getColorsNumber() ; ++i)
    {
        std::cout << "Color[" << table[i].first << "] : " << table[i].second << std::endl;
    }

	for(int i = 0 ; i < containerSet->getColorsNumber() ; ++i)
	{
        organizeColor(table,table[i].first);
        break;
	}

}

/*
 * Metoda realizujaca sprzatanie konkretnego koloru w pudelkach.
 */
template <int(*T)(std::pair<unsigned int, unsigned int> *, unsigned int) >
void ContainersSweep<T>::organizeColor(std::pair<unsigned int, unsigned int> * table, unsigned int colorNo)
{
    //tablica wartosci logicznych
    bool * isCleaned[containerSet->size()];

    containerSet->getMaxiumWithColor(colorNo, T);

}


#endif
