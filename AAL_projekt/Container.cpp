#include "Container.h"

/*
 * Konstruktor przyjmuj¹cy wektor z zawieranymi kolorami w tym pojemniku.
 */
Container::Container(const std::vector<unsigned int> & included,
	                 const unsigned int capacity,
					 const unsigned int colorsNumber) : capacity(capacity), leftPlace(capacity)
{
	for (unsigned int i = 0; i < colorsNumber; i++)
	{
		content.insert({ i, 0 });
	}

	for (size_t i = 0; i < included.size(); i++)
	{
		leftPlace--;
		content[included[i]]++;
	}
}

/*
 * Konstruktor kopiuj¹cy wykonuj¹cy g³êbok¹ kopie danych.
 */
Container::Container(const Container& another) : capacity(another.capacity), leftPlace(another.leftPlace)
{
	content = another.content;
}

/*
 * Przeniesienie klocka o kolorze blockColor do pojemnika destiny.
 * False zwracany jeœli : 
 * - klocka o danym kolorze nie ma w pojemniku this.
 * - klocek nie moze zostac przeniesiony do destiny, poniewa¿ nie ma tam miejsca.
 * 
 */
bool Container::moveBlock(unsigned int blockColor, Container& destiny)
{
	return false;
}

/*
 * Destruktor.
 */
Container::~Container(){ }

/*
 * Wyœwietl informacje na temat tego pojemnika.
 */
void Container::showInfo() const
{
	std::cout << "Capacity : " << capacity << " leftPlace : " << leftPlace << " state : " << this->checkState() << std::endl;
	for (auto x : content)
	{
		std::cout << x.first << " : " << x.second << std::endl;
	}
}

/*
 * Sprawdza czy pojemnik spe³nia warunki zadania.
 */
bool Container::checkState() const
{
	for (auto x : content)
	{
		if (x.second > 1)
		{
			return false;
		}
	}
	return true;
}

/*
 * Pobranie pojemnoœci pojemnika.
 */
unsigned int Container::getCapacity() const
{
	return capacity;
}

/*
 * Pobranie ilosci klocków koloru color znajdujacych sie w pojemniku.
 */
unsigned int Container::getColorMultiplicity(unsigned int color) const
{
	return content.at(color);
}