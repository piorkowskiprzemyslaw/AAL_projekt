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

/**
 * Zamiana klocka koloru firstColor z pojemnika firstBlockLocation z klockiem koloru
 * secondColor z pojemnika secondColorLocation za pomoca wolnego miejsca znajdujacego
 * sie w pojemniku freeSpace.
 * Zwraca ilosc dokonanych ruchow.
 */
unsigned int ContainerSet::swapBlocksWithFreeSpace(iterator & freeSpace, iterator & firstBlockLocation, Color & firstColor, iterator & secondBlockLocation, Color & secondColor)
{
	// Licznik wykonanych ruchow.
	unsigned int counter = 0;

	// Nie ma koloru firstColor w pojemniku firstBlockLocation
	if(firstBlockLocation->getColorMultiplicity(firstColor) == 0)
	{
		return counter;
	}

	// Nie ma koloru secondColor w pojemniku secondBlockLocation
	if(secondBlockLocation->getColorMultiplicity(secondColor) == 0)
	{
		return counter;
	}

	// Nie ma wolnego miejsca w pojemniku freeSpace
	if(freeSpace->getLeftPlace() == 0)
	{
		return counter;
	}

	// Wolne miejsce znajduje sie w pojemniku zawierajacym pierwszy klocek
	if(freeSpace->getIndex() == firstBlockLocation->getIndex())
	{
		Direction dir;
		unsigned int distance = 0;
		iterator tmpIter(firstBlockLocation);
		std::vector<Color * > shiftVector(containers.size(), nullptr);

		// Okreslenie optymalnego kierunku obrotow.
		if(getDistance(left, firstBlockLocation, secondBlockLocation) < getDistance(right, firstBlockLocation, secondBlockLocation))
		{
			// blizej jest patrzac w lewa strone, wiec musimy przekrecac w prawo.
			dir = right;
		} else {
			// blizej jest patrzac w prawa strone, wiec musimy przekrecac w lewo.
			dir = left;
		}

		if(dir == left)
			distance = getDistance(right, firstBlockLocation, secondBlockLocation);
		else
			distance = getDistance(left, firstBlockLocation, secondBlockLocation);

		//przygotowanie vectora przesuniecia.

		shiftVector[firstBlockLocation->getIndex()] = *firstColor;
		shiftVector[secondBlockLocation->getIndex()] = *secondColor;

		for(size_t i = 0 ; i < containers.size() ; ++i)
		{
			if(freeSpace->getIndex() != i && shiftVector[i] == nullptr)
			{
				shiftVector[i] = containers[i].getTopColor();
			}
		}

		for(size_t i = 0 ; i < shiftVector.size() ; ++i)
		{
			std::cout << "shiftVector[" << i << "] = " << shiftVector[i] << std::endl;
		}

		// wykonaj distance razy przesuniecie wszystkich klockow o jeden w kierunku dir.
		for(unsigned int i = 0 ; i < distance ; ++i)
		{
			// metoda przesuwająca.
		}

		// jesli przekrecalismy w prawo, to interesujacy klocek do zmianay bedzie znajdowal sie po lewej stronie firstBlockLocation.
		if(dir == right)
		{
			--tmpIter;
		}
		// jesli przekrecalismy w lewo, to interesujacy klocek do zmianay bedzie znajdowal sie po prawej stronie firstBlockLocation.
		else /* (dir == left) */ {
			++tmpIter;
		}

		tmpIter->moveBlock(secondColor,*firstBlockLocation);
		firstBlockLocation->moveBlock(firstColor,*tmpIter);

		// wykonaj przesuniecie wszystkich klockow o jeden w kierunku przeciwnym do dir distance razy.
		if(dir == right)
		{
			for(unsigned int i = 0 ; i < distance ; ++i)
			{
				// przesuwanie w kierunku left.
			}
		} else /* (dir == left) */ {
			for(unsigned int i = 0 ; i < distance ; ++i)
			{
				// przesuwanie w kierunku right.
			}
		}


	} else if (freeSpace->getIndex() == secondBlockLocation->getIndex())
	// wolne miejsce znajduje sie w pojemniku zawierajacym drugi klocek.
	{
		Direction dir;
		unsigned int distance = 0;
		iterator tmpIter(secondBlockLocation);

		// Okreslenie optymalnego kierunku obrotow.
		if(getDistance(left, secondBlockLocation, firstBlockLocation) < getDistance(right, secondBlockLocation, firstBlockLocation))
		{
			// blizej jest patrzac w lewa strone, wiec musimy przekrecac w prawo.
			dir = right;
		} else {
			// blizej jest patrzac w prawa strone, wiec musimy przekrecac w lewo.
			dir = left;
		}

		if(dir == left)
			distance = getDistance(right, secondBlockLocation, firstBlockLocation);
		else
			distance = getDistance(left, secondBlockLocation, firstBlockLocation);


		// wykonaj distance razy przesuniecie wszystkich klockow o jeden w kierunku dir.
		for(unsigned int i = 0 ; i < distance ; ++i)
		{
			// metoda przesuwająca.
		}

		// jesli przekrecalismy w prawo, to interesujacy klocek do zmianay bedzie znajdowal sie po lewej stronie firstBlockLocation.
		if(dir == right)
		{ // TODO : do sprawdzenia, czy na pewno w tym przypadku, to lewy sąsiad bedzie okej!!!!
			--tmpIter;
		}
		// jesli przekrecalismy w lewo, to interesujacy klocek do zmianay bedzie znajdowal sie po prawej stronie firstBlockLocation.
		else /* (dir == left) */ {
			++tmpIter;
		}

		tmpIter->moveBlock(firstColor,*secondBlockLocation);
		secondBlockLocation->moveBlock(secondColor,*tmpIter);

		// wykonaj przesuniecie wszystkich klockow o jeden w kierunku przeciwnym do dir distance razy.
		if(dir == right)
		{
			for(unsigned int i = 0 ; i < distance ; ++i)
			{
				// przesuwanie w kierunku left.
			}
		} else /* (dir == left) */ {
			for(unsigned int i = 0 ; i < distance ; ++i)
			{
				// przesuwanie w kierunku right.
			}
		}

	} else
	// wolne miejsce znajduje sie w innym pojemniku.
	{
		// firstBlockLocation jest pojemnikiem o pojemnosci wiekszej od 1, za pomoca jego mozna wykonac zamiane.
		if(firstBlockLocation->getCapacity() > 1)
		{
			Direction dir;
			unsigned int distance = 0;


			if(getDistance(left, firstBlockLocation, freeSpace) < getDistance(right, firstBlockLocation, freeSpace))
			{
				// Blizej jest patrzac w lewa strone, dlatego bedziemy musieli przesunac w prawa.
				dir = right;
			} else {
				//Blizej jest patrzac w prawa strone, dlatego bedziemy musieli przesunac w lewo.
				dir = left;
			}

			if(dir == left)
				distance = getDistance(right, firstBlockLocation, freeSpace);
			else
				distance = getDistance(left, firstBlockLocation, freeSpace);

			// teraz nalezy wykonac distance przesuniec w kierunku dir. dzieki temu
			// wolne miejsce znajdzie sie w pojemniku firstBlockLocation

			// Dalej mozemy postepowac tak jak w jednym z dwoch poprzednich wypadkow.
			counter += swapBlocksWithFreeSpace(firstBlockLocation, firstBlockLocation, firstColor, secondBlockLocation, secondColor);

			// teraz nalzey wykonac distance przesuniec w kierunku przeciwnym do dir.
			// dzieki temu wrocimy do stanu poczatkowego.
		}
		// secondBlockLocation jest pojemnikiem o pojemnosci wiekszej od 1, za pomoca jego mozna wykonanac zaimane.
		else if (secondBlockLocation->getCapacity() > 1)
		{
			Direction dir;
			unsigned int distance = 0;


			if(getDistance(left, secondBlockLocation, freeSpace) < getDistance(right, secondBlockLocation, freeSpace))
			{
				// Blizej jest patrzac w lewa strone, dlatego bedziemy musieli przesunac w prawa.
				dir = right;
			} else {
				//Blizej jest patrzac w prawa strone, dlatego bedziemy musieli przesunac w lewo.
				dir = left;
			}

			if(dir == left)
				distance = getDistance(right, secondBlockLocation, freeSpace);
			else
				distance = getDistance(left, secondBlockLocation, freeSpace);

			// teraz nalezy wykonac distance + 1 przesuniec w kierunku dir. dzieki temu
			// wolne miejsce znajdzie sie w pojemniku firstBlockLocation

			// Dalej mozemy postepowac tak jak w jednym z dwoch poprzednich wypadkow.
			counter += swapBlocksWithFreeSpace(secondBlockLocation, firstBlockLocation, firstColor, secondBlockLocation, secondColor);

			// teraz nalzey wykonac distance + 1 przesuniec w kierunku przeciwnym do dir.
			// dzieki temu wrocimy do stanu poczatkowego.
		}
		// obydwa pojemniki maja niewystarczajace pojemnosci. Trzeba znalezc pojemnik o pojemnosci > 1.
		else
		{
			Direction dir;
			ContainerSet::iterator iter = begin();
			unsigned int distance = 0;

			if(iter->getCapacity() == 1)
			{
				++iter;
				for(; iter != begin() ; ++iter)
				{
					if(iter->getCapacity() > 1)
						break;
				}
			}

			if(getDistance(left, iter, freeSpace) < getDistance(right, iter, freeSpace))
			{
				// Blizej jest patrzac w lewa strone, dlatego bedziemy musieli przesunac w prawa.
				dir = right;
			} else {
				//Blizej jest patrzac w prawa strone, dlatego bedziemy musieli przesunac w lewo.
				dir = left;
			}

			if(dir == left)
				distance = getDistance(right, iter, freeSpace);
			else
				distance = getDistance(left, iter, freeSpace);

			//jesli distance == 0 to nic wiecej nie msuismy robic.
			if(distance != 0 )
			{
				// wykoanianie distance + 1 przesuniec w kierunku dir.
			}

			// przesunieciecie klockow tak, zeby jeden z tych ktore nalezy zamienic znajdowal sie w iter

			// wywolanie rekurencyjne z blokiem do zamienienia  w tej samej lokalizacji co wolne miejsce.

			// cofniecie przesuniecia klockow

			if(distance != 0)
			{
				// wykonanie distance + 1 przesuniec w kierunku przeciwnym do dir.
			}

		}
	}

	return true;
}

/**
 * Ilosc pojemnikow dzielaca first i second w kierunku dir.
 */
unsigned int ContainerSet::getDistance(Direction dir, iterator & first, iterator & second) const
{
	unsigned int count = 0;
	if(dir == left)
	{
		for(iterator it = --first ; it != second ; --it)
		{
			++count;
		}
	} else /* (dir == right) */ {

		for(iterator it = ++first ; it != second ; ++it)
		{
			++count;
		}
	}
	return count;
}
