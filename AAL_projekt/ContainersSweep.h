#ifndef CONTAINERSSWEEP_H
#define CONTAINERSSWEEP_H

#include <iostream>
#include <cstring>
#include "ContainerSet.h"
#include "ColorCompare.h"

/*
 * Klasa realizujaca podejscie do rozwiazania problemu za pomoca omiatania pojemnikow z klockow
 * o kolorach ktore juz nie moga sie w tym pojemniku znajdowac.
 */
template <typename T>
class ContainersSweep
{
private:
	ContainerSet * containerSet;
	size_t * colorMultiplicity;
	bool checkPreconditions();
	void organizeColor(std::map<Color, bool, ColorCompare> & table, Color & color);
	T sort;
public:
	ContainersSweep();
	ContainersSweep(const ContainerSet & containerSet);
	~ContainersSweep();
	void solveProblem();
};

/*
* Domyslny konstruktor.
*/
template <typename T >
ContainersSweep<T>::ContainersSweep()
{
	containerSet = nullptr;
	colorMultiplicity = nullptr;
}

/*
* Konstrutkor pobierajacy konkretny zbior pojemnikow.
*/
template <typename T>
ContainersSweep<T>::ContainersSweep(const ContainerSet & containerSet)
{
	this->containerSet = new ContainerSet(containerSet);
	colorMultiplicity = new size_t[this->containerSet->getColorsNumber()];
}

/*
* Destruktor.
*/
template <typename T>
ContainersSweep<T>::~ContainersSweep()
{
	delete(containerSet);
	delete(colorMultiplicity);
}

/*
* Sprawdz warunki poczatkowe.
*/
template <typename T>
bool ContainersSweep<T>::checkPreconditions()
{
    unsigned int allColorSum = 0;

	//liczba kolorow musi byc mniejsza badz rowna liczbe dostepnych pojemnikow.
	if (Color::getNumberOfAllColors() > this->containerSet->size())
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
template <typename T>
void ContainersSweep<T>::solveProblem()
{
	// mapowanie koloru na stan uporzadkowania.
	std::map<Color, bool , ColorCompare> colorOrganizeMap;
    //talibca z parami <kolor, ilosc wystapien> potrzebna do zrealizowania algorytmu.
    std::vector<std::pair<Color, unsigned int> > table(0);

    // Inicjalizowanie mapy, na poczatku zaden kolor nie jest uporzadkowany.
    for(int i = 0 ; i < Color::getNumberOfAllColors() ; ++i)
    {
    	colorOrganizeMap.emplace(i , false);
    }

    // Inicjalizowanie vectora.
    for(int i = 0 ; i < Color::getNumberOfAllColors() ; ++i)
    {
    	table.emplace_back(i , containerSet->colorMultiplicity(i));
    }

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

	sort(table);

	//testowe wypisanie tabeli
	for(int i = 0 ; i < containerSet->getColorsNumber() ; ++i)
    {
        std::cout << "Color[" << table[i].first.getColor() << "] : " << table[i].second << std::endl;
    }


	//organizowanie kolorów, od najbardziej licznego do najmniej.
	for(int i = 0 ; i < table.size() ; ++i)
	{
		colorOrganizeMap[table[i].first] = true;
        organizeColor(colorOrganizeMap ,table[i].first);
        break;
	}
}

/*
 * Metoda realizujaca sprzatanie konkretnego koloru w pudelkach.
 * Table to vector wartosci logicznych mowiacy czy kolor byl juz rozpatrywany.
 */
template <typename T>
void ContainersSweep<T>::organizeColor(std::map<Color, bool, ColorCompare> & colorOrganizeMap, Color & color)
{
	//itearator na pojemnik z najwieksza liczba klockow koloru colorNo
    ContainerSet::iterator iter = containerSet->getMaxiumWithColor(color);
	//pomocniczy iterator na prawego sasiada
    ContainerSet::iterator tmpIter(iter);
	//liczba klockow koloru colorNo w iter
	unsigned int blocksNumer = 0;

    ++tmpIter;

    while(iter->getColorMultiplicity(color) >= 1)
    {
		blocksNumer = iter->getColorMultiplicity(color);

		std::cout << "Licznosc koloru " << color.getColor() << " = " << blocksNumer << std::endl;

    	//Przekladamy wszystkie nadmiarowe kulki do prawego sasiada.
    	//tutaj nastepuje rozpatrzenie przypadkow.
    	for(int i = 1 ; i < blocksNumer ; ++i)
    	{

    		// Przypadki kiedy prawy sasiad ma miejsce wolne
    		if(/*isCleaned[tmpIter()] == false*/ true)
    		{
				//Prawy sasiad ma falge flase i ma wolne miejsce
				if(tmpIter->getLeftPlace() != 0)
				{
					std::cout << "Prawy sasiad ma falge flase i ma wolne miejsce"<< std::endl;
					iter->moveBlock(color, *tmpIter);
					continue;
				}

				//Prawy sasiad ma flage false i nie ma wolnego miejsca ale u nas jest wolne miejsce
				if(tmpIter->getLeftPlace() == 0 && iter->getLeftPlace() != 0)
				{
					std::cout << "Prawy sasiad ma flage false i nie ma wolnego miejsca ale u nas jest wolne miejsce" << std::endl;

					if (tmpIter->checkIsColorPresent(colorOrganizeMap) != nullptr)
					{
						// Nie rozpatrywany kolor znajduje sie u sąsiada.
						std::cout << "Prawy sasiad ma kolor jeszcze nie rozpatrywany." << std::endl;
					}
					else {
						int colorFound = 0;
						// Nie rozpatrywany kolor nie znadjuje sie u sąsiada, szukamy na prawo
						std::cout << "Prawy sasiad nie ma koloru jeszcze nie rozpatrywanego, szukam w prawych sasiadach." << std::endl;
						while (true)
						{
							++tmpIter;
							if (tmpIter->checkIsColorPresent(colorOrganizeMap) != nullptr)
								break;
						}
						//std::cout << "Znaleziono nierozpatrywany kolor nr : " << colorFound << " w kubelku numer " << tmpIter() << std::endl;
					}
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

//		isCleaned[iter()] = true;
    	iter = containerSet->getMaxiumWithColor(color);
    	tmpIter = iter;
    	tmpIter++;
    }
}

#endif
