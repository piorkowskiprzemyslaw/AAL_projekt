#include "ContainerSet.h"

/*
 * Konstruktor ktory przyjmuje pojemnosci pojemnikow, listy zawierania oraz ilosc roznych kolorow.
 */
ContainerSet::ContainerSet(const std::vector<unsigned int> & containersCapacity, const std::vector<std::vector<unsigned int> > & includingList, const unsigned int colorsNumber)
{
	if (containersCapacity.size() != includingList.size())
	{
		std::cerr << "Internal error!" << std::endl;
		exit(1);
	}
	containers.clear();
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
	capacitySum = another.capacitySum;
	containers.clear();
	Color::setNumberOfAllColors(another.getColorsNumber());
	for(auto container : another.containers)
	{
		containers.emplace_back(container);
	}
}

/*
 * Destruktor nie ma potrzeby niszczenia niczego, poniewaz nie przydzielamy dynamicznie pamieci.
 */
ContainerSet::~ContainerSet() { }

/*
 * Wyswietl informacje na temat tego zbioru kontenerow.
 */
void ContainerSet::showInfo() const
{
	for(auto container : containers)
	{
		container.showInfo();
	}
}

/*
 * Pobierz ilosc roznych kolorow.
 */
unsigned int ContainerSet::getColorsNumber() const
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
unsigned int ContainerSet::colorMultiplicity(const Color & color) const
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
ContainerSet::iterator ContainerSet::getMaxiumWithColor(const Color & color)
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
		std::vector<Color *> shiftVector(containers.size(), nullptr);
		// Okreslenie optymalnego kierunku obrotow.
		if(getDistance(left, firstBlockLocation, secondBlockLocation) < getDistance(right, firstBlockLocation, secondBlockLocation))
		{// blizej jest patrzac w lewa strone, wiec musimy przekrecac w prawo.
			dir = right;
		} else { // blizej jest patrzac w prawa strone, wiec musimy przekrecac w lewo.
			dir = left;
		}

		if(dir == left)
			distance = getDistance(right, firstBlockLocation, secondBlockLocation);
		else
			distance = getDistance(left, firstBlockLocation, secondBlockLocation);

		if (distance > 0)
		{
			shiftVector[secondBlockLocation->getIndex()] = new Color(secondColor.getColor());

			for(size_t i = 0 ; i < containers.size() ; ++i)
			{
				if(freeSpace->getIndex() != i && shiftVector[i] == nullptr)
				{
					shiftVector[i] = new Color(containers[i].getTopColor()->getColor());
				}
			}
			//testowe wypisanie wszystkich wskaznikow w tablicy.
			//for(size_t i = 0 ; i < shiftVector.size() ; ++i)
			//{
			//	std::cout << "shiftVector[" << i << "] = " << shiftVector[i] << std::endl;
			//}

			// wykonaj distance razy przesuniecie wszystkich klockow o jeden w kierunku dir.
			counter += shiftBlocks(shiftVector, freeSpace, distance * containers.size(), dir);

			// jesli przekrecalismy w prawo, to interesujacy klocek do zmianay bedzie znajdowal sie po lewej stronie firstBlockLocation.
			if(dir == right)
			{
				--tmpIter;
			}
			// jesli przekrecalismy w lewo, to interesujacy klocek do zmianay bedzie znajdowal sie po prawej stronie firstBlockLocation.
			else /* (dir == left) */ {
				++tmpIter;
			}
		}

		tmpIter->moveBlock(secondColor,*firstBlockLocation);
		firstBlockLocation->moveBlock(firstColor,*tmpIter);
		counter += 2;
		
		if (distance != 0)
		{
			delete(shiftVector[tmpIter->getIndex()]);
			shiftVector[tmpIter->getIndex()] = new Color(firstColor.getColor());

			// wykonaj przesuniecie wszystkich klockow o jeden w kierunku przeciwnym do dir distance razy.
			if(dir == right)
			{
				counter += shiftBlocks(shiftVector, freeSpace, distance * containers.size(), left);
			} else /* (dir == left) */ {
				counter += shiftBlocks(shiftVector, freeSpace, distance * containers.size(), right);
			}

			for (auto i = 0; i < shiftVector.size(); ++i)
			{
				delete(shiftVector[i]);
			}
		}
	} else if (freeSpace->getIndex() == secondBlockLocation->getIndex()){
		Direction dir;
		unsigned int distance = 0;
		iterator tmpIter(secondBlockLocation);
		std::vector<Color *> shiftVector(containers.size(), nullptr);
		// Okreslenie optymalnego kierunku obrotow.
		if(getDistance(left, secondBlockLocation, firstBlockLocation) < getDistance(right, secondBlockLocation, firstBlockLocation))
		{// blizej jest patrzac w lewa strone, wiec musimy przekrecac w prawo.
			dir = right;
		} else {// blizej jest patrzac w prawa strone, wiec musimy przekrecac w lewo.
			dir = left;
		}

		if(dir == left)
			distance = getDistance(right, secondBlockLocation, firstBlockLocation);
		else
			distance = getDistance(left, secondBlockLocation, firstBlockLocation);

		if (distance != 0)
		{
			shiftVector[firstBlockLocation->getIndex()] = new Color(firstColor.getColor());

			for (size_t i = 0; i < containers.size(); ++i)
			{
				if (freeSpace->getIndex() != i && shiftVector[i] == nullptr)
				{
					shiftVector[i] = new Color(containers[i].getTopColor()->getColor());
				}
			}

			// wykonaj distance razy przesuniecie wszystkich klockow o jeden w kierunku dir.
			counter += shiftBlocks(shiftVector, freeSpace, distance * containers.size(), dir);
			std::cout << "After shiftBlocks with distance = " << distance << " and direction = " << dir << std::endl;
			showInfo();
		
			// jesli przekrecalismy w prawo, to interesujacy klocek do zmianay bedzie znajdowal sie po lewej stronie firstBlockLocation.
			if(dir == right)
			{ // TODO : do sprawdzenia, czy na pewno w tym przypadku, to lewy sasiad bedzie okej!!!!
				--tmpIter;
			}
			// jesli przekrecalismy w lewo, to interesujacy klocek do zmianay bedzie znajdowal sie po prawej stronie firstBlockLocation.
			else /* (dir == left) */ {
				++tmpIter;
			}
		}


		tmpIter->moveBlock(firstColor,*secondBlockLocation);
		secondBlockLocation->moveBlock(secondColor,*tmpIter);
		counter += 2;

		if (distance != 0)
		{
			delete(shiftVector[tmpIter->getIndex()]);
			shiftVector[tmpIter->getIndex()] = new Color(secondColor.getColor());
			// wykonaj przesuniecie wszystkich klockow o jeden w kierunku przeciwnym do dir distance razy.
			if(dir == right)
			{
				counter += shiftBlocks(shiftVector, freeSpace, distance * containers.size(), left);
			} else /* (dir == left) */ {
				counter += shiftBlocks(shiftVector, freeSpace, distance * containers.size(), right);
			}
			
			for (auto i = 0; i < shiftVector.size(); ++i)
			{
				delete(shiftVector[i]);
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
			std::vector<Color *> shiftVector(containers.size(), nullptr);
			iterator tmpSecondBlockLocation(secondBlockLocation);
			Color * tmpColor;

			if(getDistance(left, firstBlockLocation, freeSpace) < getDistance(right, firstBlockLocation, freeSpace))
			{
				dir = left;
			} else {
				dir = right;
			}

			distance = getDistance(dir, firstBlockLocation, freeSpace);
			if (getDistance(dir, firstBlockLocation, tmpSecondBlockLocation) < distance)
			{
				if (dir == left)
					--tmpSecondBlockLocation;
				else
					++tmpSecondBlockLocation;
			}

			shiftVector[firstBlockLocation->getIndex()] = new Color(firstBlockLocation->getTopColor(&firstColor)->getColor());

			for (auto i = 0; i < containers.size(); ++i)
			{
				if (freeSpace->getIndex() != i && shiftVector[i] == nullptr)
					shiftVector[i] = new Color(containers[i].getTopColor()->getColor());
			}

			// teraz nalezy wykonac distance przesuniec w kierunku dir. dzieki temu
			// wolne miejsce znajdzie sie w pojemniku firstBlockLocation
			counter += shiftBlocks(shiftVector, freeSpace, distance + 1, dir);

			std::cout << "Po przygotowujacym shifcie..." << std::endl;
			showInfo();

			// Dalej mozemy postepowac tak jak w jednym z dwoch poprzednich wypadkow.
			counter += swapBlocksWithFreeSpace(firstBlockLocation, firstBlockLocation, firstColor, tmpSecondBlockLocation, secondColor);

			std::cout << std::endl << "Po zamianie miejsc za pomoca swapBlocksWithFreeSpace ..." << std::endl;
			showInfo();

			// teraz nalzey wykonac distance przesuniec w kierunku przeciwnym do dir.
			// dzieki temu wrocimy do stanu poczatkowego.
			tmpColor = shiftVector[tmpSecondBlockLocation->getIndex()];
			delete(tmpColor);
			shiftVector[tmpSecondBlockLocation->getIndex()] = new Color(firstColor.getColor());

			if (dir == left)
				counter += shiftBlocks(shiftVector, firstBlockLocation, distance + 1, right);
			else
				counter += shiftBlocks(shiftVector, firstBlockLocation, distance + 1, left);

			for (auto i = 0; i < shiftVector.size(); ++i)
			{
				delete(shiftVector[i]);
			}	

			std::cout << std::endl << "Po doprowadzeniu do poczatkowego stanu... " << std::endl;
			showInfo();
		}
		// secondBlockLocation jest pojemnikiem o pojemnosci wiekszej od 1, za pomoca jego mozna wykonanac zaimane.
		else if (secondBlockLocation->getCapacity() > 1)
		{
			Direction dir;
			unsigned int distance = 0;
			std::vector<Color *> shiftVector(containers.size(), nullptr);
			iterator tmpFirstBlockLocation(firstBlockLocation);
			Color * tmpColor;

			if(getDistance(left, secondBlockLocation, freeSpace) < getDistance(right, secondBlockLocation, freeSpace))
			{
				dir = left;
			} else {
				dir = right;
			}

			distance = getDistance(dir, secondBlockLocation, freeSpace);
			if (getDistance(dir, secondBlockLocation, tmpFirstBlockLocation) < distance)
			{
				if (dir == left)
					--tmpFirstBlockLocation;
				else
					++tmpFirstBlockLocation;
			}

			shiftVector[secondBlockLocation->getIndex()] = new Color(secondBlockLocation->getTopColor(&secondColor)->getColor());
			for (auto i = 0; i < containers.size(); ++i)
			{
				if (freeSpace->getIndex() != i && shiftVector[i] == nullptr)
					shiftVector[i] = new Color(containers[i].getTopColor()->getColor());
			}
			// teraz nalezy wykonac distance + 1 przesuniec w kierunku dir. dzieki temu
			// wolne miejsce znajdzie sie w pojemniku firstBlockLocation
			counter += shiftBlocks(shiftVector, freeSpace, distance + 1, dir);

			std::cout << "Po przygotowujacym shifcie..." << std::endl;
			showInfo();

			// Dalej mozemy postepowac tak jak w jednym z dwoch poprzednich wypadkow.
			counter += swapBlocksWithFreeSpace(secondBlockLocation, tmpFirstBlockLocation, firstColor, secondBlockLocation, secondColor);

			std::cout << std::endl << "Po zamianie miejsc za pomoca swapBlocksWithFreeSpace ..." << std::endl;
			showInfo();

			// teraz nalzey wykonac distance + 1 przesuniec w kierunku przeciwnym do dir.
			// dzieki temu wrocimy do stanu poczatkowego.
			tmpColor = shiftVector[tmpFirstBlockLocation->getIndex()];
			delete(tmpColor);
			shiftVector[tmpFirstBlockLocation->getIndex()] = new Color(secondColor.getColor());

			if (dir == left)
				counter += shiftBlocks(shiftVector, secondBlockLocation, distance + 1, right);
			else
				counter += shiftBlocks(shiftVector, secondBlockLocation, distance + 1, left);

			for (auto i = 0; i < shiftVector.size(); ++i)
			{
				delete(shiftVector[i]);
			}

			std::cout << std::endl << "Po doprowadzeniu do poczatkowego stanu... " << std::endl;
			showInfo();
		}
		// obydwa pojemniki maja niewystarczajace pojemnosci. Trzeba znalezc pojemnik o pojemnosci > 1.
		else
		{
			Direction dir;
			iterator iter = begin();
			unsigned int distance = 0;
			unsigned int dstFirstR = 0, dstFirstL = 0, dstSecR = 0, dstSecL = 0;
			std::vector<Color *> shiftVector1(containers.size(), nullptr);
			iterator tmpFirstBlockLocation(firstBlockLocation);
			iterator tmpSecondBlockLocation(secondBlockLocation);
			iterator tmp(freeSpace);
			Color * tmpIterColor;

			if (freeSpace->getCapacity() <= 1)
			{
				//szukanie pojemnika o pojemnosci wiekszej od 1.
				for (auto i = 0; i < containers.size(); ++i, ++iter)
				{
					if (iter->getCapacity() > 1)
						break;
				}
			}
			else {
				iter = freeSpace;
			}

			if (getDistance(left, iter, freeSpace) < getDistance(right, iter, freeSpace))
			{
				dir = left;
			} else {
				dir = right;
			}

			//przemieszczanie wolnego miejsca do tego pojemnika.
			distance = getDistance(dir, iter, freeSpace);

			if (distance != 0)
			{

				shiftVector1[tmpFirstBlockLocation->getIndex()] = new Color(firstColor.getColor());
				shiftVector1[tmpSecondBlockLocation->getIndex()] = new Color(secondColor.getColor());

				for (auto i = 0; i < containers.size(); ++i)
				{
					if (freeSpace->getIndex() != i && shiftVector1[i] == nullptr)
						shiftVector1[i] = new Color(containers[i].getTopColor()->getColor());
				}

				if (getDistance(dir, iter, tmpFirstBlockLocation) < distance)
				{
					if (dir == left)
						--tmpFirstBlockLocation;
					else
						++tmpFirstBlockLocation;
				}

				if (getDistance(dir, iter, tmpSecondBlockLocation) < distance)
				{
					if (dir == left)
						--tmpSecondBlockLocation;
					else
						++tmpSecondBlockLocation;
				}

				counter += shiftBlocks(shiftVector1, freeSpace, distance + 1, dir);
				std::cout << "Need to shift blocks at the beginning, result..." << std::endl;
				showInfo();
			}


			// przesunieciecie klockow tak, zeby jeden z tych ktore nalezy zamienic znajdowal sie w iter.
			// wywolanie rekurencyjne z blokiem do zamienienia  w tej samej lokalizacji co wolne miejsce.
			dstFirstR = getDistance(right, iter, tmpFirstBlockLocation);
			dstFirstL = getDistance(left, iter, tmpFirstBlockLocation);
			dstSecR = getDistance(right, iter,tmpSecondBlockLocation);
			dstSecL = getDistance(left, iter, tmpSecondBlockLocation);
			
			if (dstFirstR <= dstFirstL && dstFirstR <= dstSecR && dstFirstR <= dstSecL)
			{
				tmpIterColor = new Color(iter->getTopColor()->getColor());
				counter += swapBlocksWithFreeSpace(iter, tmpFirstBlockLocation, firstColor, iter, *tmpIterColor);
				tmp = iter;
				iter = tmpFirstBlockLocation;
				tmpFirstBlockLocation = tmp;

				std::cout << "After helpfull swap with iter..." << std::endl;
				showInfo();

				counter += swapBlocksWithFreeSpace(tmpFirstBlockLocation, tmpFirstBlockLocation, firstColor, tmpSecondBlockLocation, secondColor);
				tmp = tmpFirstBlockLocation;
				tmpFirstBlockLocation = tmpSecondBlockLocation;
				tmpSecondBlockLocation = tmp;

				std::cout << "After main swap between colors..." << std::endl;
				showInfo();

				counter += swapBlocksWithFreeSpace(tmpSecondBlockLocation, tmpSecondBlockLocation, secondColor, iter, *tmpIterColor);
				tmp = tmpSecondBlockLocation;
				tmpSecondBlockLocation = iter;
				iter = tmp;

				delete(tmpIterColor);

				std::cout << "After reverting helpful swap..." << std::endl;
				showInfo();
			}
			else if (dstFirstL <= dstFirstR && dstFirstL <= dstSecR && dstFirstL <= dstSecL)
			{
				tmpIterColor = new Color(iter->getTopColor()->getColor());
				counter += swapBlocksWithFreeSpace(iter, tmpFirstBlockLocation, firstColor, iter, *tmpIterColor);
				tmp = iter;
				iter = tmpFirstBlockLocation;
				tmpFirstBlockLocation = tmp;

				std::cout << "After helpfull swap with iter..." << std::endl;
				showInfo();

				counter += swapBlocksWithFreeSpace(tmpFirstBlockLocation, tmpFirstBlockLocation, firstColor, tmpSecondBlockLocation, secondColor);
				tmp = tmpFirstBlockLocation;
				tmpFirstBlockLocation = tmpSecondBlockLocation;
				tmpSecondBlockLocation = tmp;

				std::cout << "After main swap between colors..." << std::endl;
				showInfo();

				counter += swapBlocksWithFreeSpace(tmpSecondBlockLocation, tmpSecondBlockLocation, secondColor, iter, *tmpIterColor);
				tmp = tmpSecondBlockLocation;
				tmpSecondBlockLocation = iter;
				iter = tmp;

				delete(tmpIterColor);

				std::cout << "After reverting helpful swap..." << std::endl;
				showInfo();
			}
			else if (dstSecR <= dstFirstR && dstSecR <= dstFirstL && dstSecR <= dstSecL)
			{
				tmpIterColor = new Color(iter->getTopColor()->getColor());
				counter += swapBlocksWithFreeSpace(iter, tmpSecondBlockLocation, secondColor, iter, *tmpIterColor);
				tmp = iter;
				iter = tmpSecondBlockLocation;
				tmpSecondBlockLocation = iter;

				std::cout << "After helpfull swap with iter..." << std::endl;
				showInfo();

				counter += swapBlocksWithFreeSpace(tmpSecondBlockLocation, tmpSecondBlockLocation, secondColor, tmpFirstBlockLocation, firstColor);
				tmp = tmpFirstBlockLocation;
				tmpFirstBlockLocation = tmpSecondBlockLocation;
				tmpSecondBlockLocation = tmp;

				std::cout << "After main swap between colors..." << std::endl;
				showInfo();

				counter += swapBlocksWithFreeSpace(tmpFirstBlockLocation, tmpFirstBlockLocation, firstColor, iter, *tmpIterColor);
				tmp = tmpFirstBlockLocation;
				tmpFirstBlockLocation = iter;
				iter = tmp;

				delete(tmpIterColor);

				std::cout << "After reverting helpful swap..." << std::endl;
				showInfo();
			}
			else if (dstSecL <= dstFirstR && dstSecL <= dstFirstL && dstSecL <= dstSecR)
			{
				tmpIterColor = new Color(iter->getTopColor()->getColor());
				counter += swapBlocksWithFreeSpace(iter, tmpSecondBlockLocation, secondColor, iter, *tmpIterColor);
				tmp = iter;
				iter = tmpSecondBlockLocation;
				tmpSecondBlockLocation = iter;

				std::cout << "After helpfull swap with iter..." << std::endl;
				showInfo();

				counter += swapBlocksWithFreeSpace(tmpSecondBlockLocation, tmpSecondBlockLocation, secondColor, tmpFirstBlockLocation, firstColor);
				tmp = tmpFirstBlockLocation;
				tmpFirstBlockLocation = tmpSecondBlockLocation;
				tmpSecondBlockLocation = tmp;

				std::cout << "After main swap between colors..." << std::endl;
				showInfo();

				counter += swapBlocksWithFreeSpace(tmpFirstBlockLocation, tmpFirstBlockLocation, firstColor, iter, *tmpIterColor);
				tmp = tmpFirstBlockLocation;
				tmpFirstBlockLocation = iter;
				iter = tmp;

				delete(tmpIterColor);

				std::cout << "After reverting helpful swap..." << std::endl;
				showInfo();
			}


			// cofniecie przesuniecia klockow
			if(distance != 0)
			{
				tmpIterColor = shiftVector1[tmpFirstBlockLocation->getIndex()];
				shiftVector1[tmpFirstBlockLocation->getIndex()] = shiftVector1[tmpSecondBlockLocation->getIndex()];
				shiftVector1[tmpSecondBlockLocation->getIndex()] = tmpIterColor;
				
				if (dir == left)
					counter += shiftBlocks(shiftVector1, iter, distance + 1, right);
				else
					counter += shiftBlocks(shiftVector1, iter, distance + 1, left);

				std::cout << "Final shift ... " << std::endl;
				showInfo();

				for (auto i = 0; i < shiftVector1.size(); ++i)
				{
					delete(shiftVector1[i]);
				}
			}

		}
	}

	return counter;
}

/**
 * Ilosc pojemnikow dzielaca first i second w kierunku dir.
 */
unsigned int ContainerSet::getDistance(Direction dir, iterator & first, iterator & second) const
{
	iterator it = first;
	unsigned int count = 0;
	if(dir == left)
	{
		--it;
		for(; it != second ; --it)
		{
			++count;
		}
	} else /* (dir == right) */ {
		++it;
		for(; it != second ; ++it)
		{
			++count;
		}
	}
	return count;
}

/*
 * Metoda przsuwajaca klocki o kolorach podanych w vectorze distance razy, przy uzyciu
 */
unsigned int ContainerSet::shiftBlocks(std::vector<Color *> & shiftVector, const iterator & freeSpace, unsigned int distance, Direction dir)
{
	unsigned int counter = 0;
	iterator sourceIter = freeSpace;
	iterator destinationIter = sourceIter;
	std::vector<Color *> vector(shiftVector);
	Color * tmp;

	if (dir == right)
	{
		sourceIter--;
		for (size_t i = 0; i < distance; i++)
		{
			tmp = vector[sourceIter->getIndex()];
			destinationIter = sourceIter;
			destinationIter++;
			
			if (tmp != nullptr)
			{
				sourceIter->moveBlock(*tmp, *destinationIter);
				++counter;
			}

			tmp = vector[sourceIter->getIndex()];
			vector[sourceIter->getIndex()] = vector[destinationIter->getIndex()];
			vector[destinationIter->getIndex()] = tmp;
			
			--sourceIter;
		}
	}
	else /* (dir == left) */{

		++sourceIter;
		for (size_t i = 0; i < distance; i++)
		{
			tmp = vector[sourceIter->getIndex()];
			destinationIter = sourceIter;
			--destinationIter;

			if (tmp != nullptr)
			{
				sourceIter->moveBlock(*tmp, *destinationIter);
				++counter;
			}

			tmp = vector[sourceIter->getIndex()];
			vector[sourceIter->getIndex()] = vector[destinationIter->getIndex()];
			vector[destinationIter->getIndex()] = tmp;
			++sourceIter;
		}
	}
	shiftVector = vector;
	return counter;
}

