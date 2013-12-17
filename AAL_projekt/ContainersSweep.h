#ifndef CONTAINERSSWEEP_H
#define CONTAINERSSWEEP_H

#include <iostream>
#include <cstring>
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
template <int (*T)(std::pair<unsigned int, unsigned int> *, unsigned int) >
void ContainersSweep<T>::solveProblem()
{
    //talibca z parami <kolor, ilosc wystapien> potrzebna do zrealizowania algorytmu.
    std::pair<unsigned int, unsigned int > * table = new std::pair<unsigned int, unsigned int>[containerSet->getColorsNumber()];
	std::cout << "Solving problem with ContainersSweep" << std::endl;
	std::cout << "Checking preconditions ... ";

	//sprawdzanie warunkow poczatkowych umozliwiajacych rozwiazanie zadania.
	if (!checkPreconditions())
	{
		std::cout << "[FALSE]" << std::endl;
		std::cout << "End of solving problem" << std::endl;
		return;
	}
	std::cout << "[OK]" << std::endl;

	//zapelnianie tablicy parami <kolor, licznosc>
	for (int i = 0; i < containerSet->getColorsNumber(); ++i)
	{
        table[i] = std::make_pair(i, colorMultiplicity[i]);
    }

	//posortowanie tableli <kolor, licznosc> po licznosciach
	T(table, containerSet->getColorsNumber());

	//testowe wypisanie tabeli
	for(int i = 0 ; i < containerSet->getColorsNumber() ; ++i)
    {
        std::cout << "Color[" << table[i].first << "] : " << table[i].second << std::endl;
    }

	//organizowanie kolorów, od najbardziej licznego do najmniej.
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
    ContainerSet::iterator iter = containerSet->getMaxiumWithColor(colorNo, T);
    ContainerSet::iterator tmpIter(iter);
    //tablica wartosci logicznych
    bool isCleaned[containerSet->size()];
    //wszystkie wartosci ustawiam na zero
    memset(isCleaned, 0, sizeof(isCleaned));

    std::cout << "Indeks : " << iter();
    ++tmpIter;

    while(iter->getColorMultiplicity(colorNo) >= 1)
    {
    	std::cout << " liczbnosc koloru " << colorNo << " = " << iter->getColorMultiplicity(colorNo) << std::endl;

    	//Przekladamy wszystkie nadmiarowe kulki do prawego sasiada.
    	//tutaj nastepuje rozpatrzenie przypadkow.
    	for(int i = 1 ; i < iter->getColorMultiplicity(colorNo) ; ++i)
    	{

    		// Przypadki kiedy prawy sasiad ma miejsce wolne
    		if(isCleaned[tmpIter()] == false)
    		{
				//Prawy sasiad ma falge flase i ma wolne miejsce
				if(tmpIter->getLeftPlace() != 0)
				{
					std::cout << "Prawy sasiad ma falge flase i ma wolne miejsce"<< std::endl;
					iter->moveBlock(colorNo, *tmpIter);
					continue;
				}

				//Prawy sasiad ma flage false i nie ma wolnego miejsca ale u nas jest wolne miejsce
				if(tmpIter->getLeftPlace() == 0 && iter->getLeftPlace() != 0)
				{
					std::cout << "Prawy sasiad ma flage false i nie ma wolnego miejsca ale u nas jest wolne miejsce" << std::endl;
					continue;
				}

				//Prawy sasaiad ma flage flase, nie ma wolnego miejsca, w akualnie rozpatrywanym
				//pojemniku tez nie ma wolnego miejsca.
				if(tmpIter->getLeftPlace() == 0 && iter->getLeftPlace() == 0)
				{
					std::cout << "Prawy sasaiad ma flage flase, nie ma wolnego miejsca, w akualnie rozpatrywanym pojemniku tez nie ma wolnego miejsca." << std::endl;
					continue;
				}
    		} else /* (isCleaned[tmpIter()] == true) */ {

    			// Prawy sasiad ma flage true i ma wolone miejsce.
    			if(tmpIter->getLeftPlace() != 0)
    			{
    				std::cout << "Prawy sasiad ma flage true i ma wolone miejsce." <<std::endl;
    				continue;
    			}

    			// Prawy sąsiad ma flage true i nie ma wolnego miejsca, ale w aktualnie rozpatrywanym
    			// pojemniku jest wolne miejsce.
    			if(tmpIter->getLeftPlace() == 0 && iter->getLeftPlace() != 0)
    			{
    				std::cout << "Prawy sąsiad ma flage true i nie ma wolnego miejsca, ale w aktualnie rozpatrywanym pojemniku jest wolne miejsce." << std::endl;
    				continue;
    			}

    			// Prawy sasiad ma flage true i nie ma wolnego miejsca, w aktualnie rozpatrywanym
    			// pojemniku tez nie ma wolnego miejsca.
    			if(tmpIter->getLeftPlace() == 0 && iter->getLeftPlace() == 0)
    			{
    				std::cout << "Prawy sasiad ma flage true i nie ma wolnego miejsca, w aktualnie rozpatrywanym pojemniku tez nie ma wolnego miejsca." << std::endl;
    				continue;
    			}

    		}

    	}

    	iter = containerSet->getMaxiumWithColor(colorNo, T);
    	tmpIter = iter;
    	tmpIter++;

    	break;
    }
}


#endif
