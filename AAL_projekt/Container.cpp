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
 * Destruktor.
 */
Container::~Container(){ }

/*
 * Wyœwietl informacje na temat tego pojemnika.
 */
void Container::showInfo()
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
bool Container::checkState()
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