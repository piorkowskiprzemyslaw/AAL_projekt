#include "Container.h"

/*
 * Konstruktor domyslny
 */
Container::Container()
{
	capacity = 0;
	blockSet = std::multiset<Block,BlockCompare>();
}

/*
 * Konstruktor przyjmujacy wektor z zawieranymi kolorami w tym pojemniku.
 */
Container::Container(const std::vector<unsigned int> & included,
	                 const unsigned int capacity) : capacity{capacity}
{
	for(unsigned int colorNo : included)
	{
		blockSet.emplace(colorNo);
	}
}

/*
 * Konstruktor kopiujacy wykonujacy gleboka kopie danych.
 */
Container::Container(const Container& another) : capacity{another.capacity}
{
	blockSet = another.blockSet;
}

/**
 * Operator przypisania.
 */
Container& Container::operator=(const Container & rhs)
{
	this->blockSet = rhs.blockSet;
	this->capacity = rhs.capacity;
	return *this;
}

/*
 * Przeniesienie klocka o kolorze blockColor do pojemnika destiny.
 * False zwracany jesli :
 * - klocka o danym kolorze nie ma w pojemniku this.
 * - klocek nie moze zostac przeniesiony do destiny, poniewaz nie ma tam miejsca.
 *
 */
bool Container::moveBlock(unsigned int blockColor, Container& destiny)
{
	// nie ma co przelozyc.
	if(getColorMultiplicity(blockColor) == 0)
		return false;

	// nie ma miejsca w destiny
	if(destiny.getLeftPlace() == 0)
		return false;

	Block block(blockColor);

	assert(0 != blockSet.erase(block));
	destiny.blockSet.emplace(block);

	return true;
}

/*
 * Destruktor.
 */
Container::~Container(){ }

/*
 * Wyswietl informacje na temat tego pojemnika.
 */
void Container::showInfo() const
{
	std::cout << "Capacity : " << capacity << " leftPlace : " << getLeftPlace() << " state : " << this->checkState() << std::endl;
	for(unsigned int i = 0 ; i < Color::getNumberOfAllColors() ; ++i)
	{
		std::cout << i << " : " << blockSet.count(Block(i)) << std::endl;
	}
}

/*
 * Sprawdza czy pojemnik spelnia warunki zadania.
 */
bool Container::checkState() const
{
	for(unsigned int i = 0 ; i < Color::getNumberOfAllColors() ; ++i)
	{
		if(blockSet.count(Block(i)) > 1)
			return false;
	}
	return true;
}

/*
 * Pobranie pojemnosci pojemnika.
 */
unsigned int Container::getCapacity() const
{
	return capacity;
}

/*
 * Pobranie ilosci klockow koloru color znajdujacych sie w pojemniku.
 */
unsigned int Container::getColorMultiplicity(unsigned int color) const
{
	return blockSet.count(Block(color));
}

/**
 * Pobranie pozostalego wolnego miejsca w kontenerach.
 */
unsigned int Container::getLeftPlace() const
{
	return (capacity - blockSet.size());
}

/**
 * Sprawdza ktory z nierozpatrywanych kolorow znajduje sie w kontenerze.
 * Jesli taki kolor istnieje to zwracany jest jego numer, jesli nie
 * to zwracane jest -1.
 */
long Container::checkIsColorPresent(std::vector<bool> & table) const
{
	return 0;
}
