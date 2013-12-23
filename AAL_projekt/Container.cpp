#include "Container.h"

/*
 * Konstruktor domyslny
 */
Container::Container()
{
	capacity = 0;
	blockSet = std::multiset<Block,BlockCompare>();
	index = 0;
}

/*
 * Konstruktor przyjmujacy wektor z zawieranymi kolorami w tym pojemniku.
 */
Container::Container(const std::vector<unsigned int> & included,
	                 const unsigned int capacity, const unsigned int index) : capacity(capacity), index(index)
{
	for(unsigned int colorNo : included)
	{
		blockSet.emplace(colorNo);
	}
}

/*
 * Konstruktor kopiujacy.
 */
Container::Container(const Container& another) : capacity(another.capacity) , index(another.index)
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
	this->index = rhs.index;
	return *this;
}

/*
 * Przeniesienie klocka o kolorze blockColor do pojemnika destiny.
 * False zwracany jesli :
 * - klocka o danym kolorze nie ma w pojemniku this.
 * - klocek nie moze zostac przeniesiony do destiny, poniewaz nie ma tam miejsca.
 *
 */
bool Container::moveBlock(Color & color, Container& destiny)
{
	// nie ma co przelozyc.
	if(getColorMultiplicity(color) == 0)
		return false;

	// nie ma miejsca w destiny
 	if(destiny.getLeftPlace() == 0)
		return false;
	
	Block block(color.getColor());

	assert(0 != blockSet.erase(block));
	destiny.blockSet.emplace(block.getColor());

	color.setColor(block.getColor()->getColor());

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
	std::cout << "Index : " << index << " capacity : " << capacity << " leftPlace : " << getLeftPlace() << " state : " << this->checkState() << std::endl;
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

/*
 * Pobranie ilosci klockow koloru color znajdujacych sie w pojemniku.
 */
unsigned int Container::getColorMultiplicity(const Color & color) const
{
	return blockSet.count(Block(color.getColor()));
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
 * Zwracany jest wskaznik na obiekt Color koloru zawieranego.
 */
Color* Container::checkIsColorPresent(std::map<Color, bool, ColorCompare> & colorOrganizeMap) const
{
	std::multiset<Block>::iterator tmpIter;

	for(auto element : colorOrganizeMap)
	{
		if(element.second == false)
		{
			tmpIter = blockSet.find(Block(element.first));
			if(blockSet.end() != tmpIter)
			{
				return tmpIter->getColor();
			}
		}
	}
	return nullptr;
}

/**
 * Pobiera index aktualnego pojemnika
 */
unsigned int Container::getIndex() const
{
	return index;
}

/*
 * Pobiera kolor pierwszego z brzegu klocka.
 */
Color* Container::getTopColor() const
{
	std::multiset<Block, BlockCompare>::iterator iter = blockSet.begin();
	if(iter == blockSet.end())
		return nullptr;
	return iter->getColor();
}
