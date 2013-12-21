#include "ContainerSet.h"

/*
 * Konstruktor ktory przyjmuje pojemnosci pojemnikow, listy zawierania oraz ilosc roznych kolorow.
 */
ContainerSet::ContainerSet(const std::vector<unsigned int> & containersCapacity,
						   const std::vector<std::vector<unsigned int> > & includingList,
						   const unsigned int colorsNumber)
{
	std::vector<Container>::iterator emplaceIterator = containers.begin();
	if (containersCapacity.size() != includingList.size())
	{
		std::cout << "Internal error!" << std::endl;
		exit(1);
	}

	containers = std::vector<Container>(0);
	Color::setNumberOfAllColors(colorsNumber);
	capacitySum = 0;

	for (size_t i = 0; i < containersCapacity.size(); ++i)
	{
		capacitySum += containersCapacity[i];
		containers.emplace_back(includingList[i], containersCapacity[i], i);
	}

}

/*
 * Konstruktor kopiujacy wykonujacy gleboka kopie danych.
 */
ContainerSet::ContainerSet(const ContainerSet& another)
{
	containers = another.containers;
	capacitySum = another.capacitySum;
}

/*
 * Destruktor niszczy obiekty stworzone w konstruktorze.
 */
ContainerSet::~ContainerSet() { }

/*
 * Wyswietl informacje na temat tego zbioru kontenerow.
 */
void ContainerSet::showInfo()
{
	for(auto container : containers)
	{
		container.showInfo();
	}
}

/*
 * Pobierz ilosc roznych kolorow.
 */
unsigned int ContainerSet::getColorsNumber()
{
	return Color::getNumberOfAllColors();
}

/*
 * Zwraca rozmiar zbioru pojemnikow.
 */
size_t ContainerSet::size() const
{
	return containers.size();
}

/*
 * Pobierz ilosc klockow w kolorze color ktore znajduja sie we wszystkich pojemnikach.
 */
unsigned int ContainerSet::colorMultiplicity(unsigned int color)
{
	unsigned int sum = 0;
	for(auto container : containers)
	{
		sum += container.getColorMultiplicity(color);
	}
	return sum;
}

/*
 * Zwraca iterator na pierwszy pojemnik.
 */
ContainerSet::iterator ContainerSet::begin()
{
	return ContainerSet::iterator(containers, 0);
}

/**
 * Zwraca sume pojemnosci wszystkich pojemnikow.
 */
size_t ContainerSet::getCapacitySum() const
{
	return capacitySum;
}

/**
 * Metoda zwracajaca iterator na pojemnik z najwieksza iloscia klockow koloru color.
 * Dziala w czasie liniowym w stosunku do pojemnosci kontenera.
 */
ContainerSet::iterator ContainerSet::getMaxiumWithColor(unsigned int color)
{
	std::vector<Container>::iterator maxWithColor = containers.begin();

	for(std::vector<Container>::iterator iter = containers.begin() + 1 ; iter != containers.end(); ++iter)
	{
		if(iter->getColorMultiplicity(color) > maxWithColor->getColorMultiplicity(color))
		{
			maxWithColor = iter;
		}
	}

    return ContainerSet::iterator(containers, maxWithColor->getIndex());
}

/**
 * Metoda zwracajaca iterator na pojemnik z najwieksza iloscia klockow koloru color.
 * Dziala w czasie liniowym w stosunku do pojemnosci kontenera.
 */
ContainerSet::iterator ContainerSet::getMaxiumWithColor(Color & color)
{
	std::vector<Container>::iterator maxWithColor = containers.begin();

	for(std::vector<Container>::iterator iter = containers.begin() + 1 ; iter != containers.end(); ++iter)
	{
		if(iter->getColorMultiplicity(color) > maxWithColor->getColorMultiplicity(color))
		{
			maxWithColor = iter;
		}
	}

    return ContainerSet::iterator(containers, maxWithColor->getIndex());
}
