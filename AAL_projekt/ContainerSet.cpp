#include "ContainerSet.h"

/*
 * Konstruktor który przyjmuje pojemnosci pojemnikow, listy zawierania oraz ilosc roznych kolorów.
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

	containers = new Container*[containersCapacity.size()];
	this->colorsNumber = colorsNumber;
	arraySize = includingList.size();
	capacitySum = 0;

	for (size_t i = 0; i < containersCapacity.size(); i++)
	{
		capacitySum += containersCapacity[i];
		containers[i] = new Container(includingList[i], containersCapacity[i], colorsNumber);
	}

}

/*
 * Konstruktor kopiuj¹cy wykonuj¹cy g³êbok¹ kopie danych.
 */
ContainerSet::ContainerSet(const ContainerSet& another)
{
	colorsNumber = another.colorsNumber;
	arraySize = another.arraySize;
	containers = new Container*[arraySize];

	for (size_t i = 0; i < arraySize; i++)
	{
		containers[i] = new Container(*another.containers[i]);
	}
}

/*
 * Destruktor niszczy obiekty stworzone w konstruktorze.
 */
ContainerSet::~ContainerSet()
{
	for (size_t i = 0; i < arraySize; i++)
	{
		delete(containers[i]);
	}

	delete[] containers;
}

/*
 * Wyswietl informacje na temat tego zbioru kontenerów.
 */
void ContainerSet::showInfo()
{
	for (size_t i = 0; i < arraySize; i++)
	{
		containers[i]->showInfo();
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
 * Zwraca rozmiar zbioru pojemników.
 */
size_t ContainerSet::size() const
{
	return arraySize;
}

/*
 * Pobierz ilosc klockow w kolorze color ktore znajduja sie we wszystkich pojemnikach.
 */
unsigned int ContainerSet::colorMultiplicity(unsigned int color)
{
	unsigned int sum = 0;
	for (size_t i = 0; i < arraySize; i++)
	{
		sum += containers[i]->getColorMultiplicity(color);
	}
	return sum;
}

/*
 * Zwraca iterator na pierwszy pojemnik.
 */
ContainerSet::iterator ContainerSet::begin()
{
	return ContainerSet::iterator(0, containers, arraySize);
}

/*
 * Zwraca const_iterator na pierwszy pojemnik.
 */
ContainerSet::const_iterator ContainerSet::begin() const
{
	return ContainerSet::const_iterator(0, containers, arraySize);
}

/*
 * Zwraca sume pojemnosci wszystkich pojemników.
 */
size_t ContainerSet::getCapacitySum() const
{
	return capacitySum;
}
