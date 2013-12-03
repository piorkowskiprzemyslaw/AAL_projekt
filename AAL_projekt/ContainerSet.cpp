#include "ContainerSet.h"

/*
 * Konstruktor kt�ry przyjmuje pojemnosci pojemnikow, listy zawierania oraz ilosc roznych kolor�w.
 */
ContainerSet::ContainerSet(const std::vector<unsigned int> & containersCapacity,
						   const std::vector<std::vector<unsigned int> > & includingList,
						   const unsigned int colorsNumber)
{
	if (containersCapacity.size() != includingList.size())
	{
		std::cout << "Internal error!" << std::endl;
		exit(1);
	}

	for (size_t i = 0; i < containersCapacity.size(); i++)
	{
		containers.push_back(new Container(includingList[i], containersCapacity[i], colorsNumber));
	}

	this->colorsNumber = colorsNumber;
	this->containersNumber = includingList.size();
}

/*
 * Konstruktor kopiuj�cy wykonuj�cy g��bok� kopie danych.
 */
ContainerSet::ContainerSet(const ContainerSet& another)
{
	for (auto x : another.containers)
	{
		containers.push_back(new Container(*x));
	}
}

/*
 * Destruktor niszczy obiekty stworzone w konstruktorze.
 */
ContainerSet::~ContainerSet()
{
	for(auto pointer : containers)
	{
		delete(pointer);
	}
}

/*
 * Wyswietl informacje na temat tego zbioru kontener�w.
 */
void ContainerSet::showInfo()
{
	for (Container * x : containers)
	{
		x->showInfo();
		std::cout << std::endl;
	}
}

/*
 * Pobierz ilosc roznych kolorow.
 */
unsigned int ContainerSet::getColorsNumber()
{
	return colorsNumber;
}

/*
 * Pobierz ilosc pojemnikow.
 */
unsigned int ContainerSet::getContainersNumer()
{
	return containersNumber;
}
