#include "Container.h"

/**
 * Konstruktor domyslny
 */
Container::Container()
{
	capacity = 0;
	blockSet = std::multiset<Block,BlockCompare>();
	index = 0;
}

/**
 * Konstruktor przyjmujacy wektor z zawieranymi kolorami w tym pojemniku.
 */
Container::Container(const std::vector<unsigned int> & included, const unsigned int capacity, const unsigned int index) : capacity(capacity), index(index)
{
	for(auto colorNo : included)
	{
		blockSet.emplace(colorNo);
	}
}

/**
 * Konstruktor kopiujacy.
 */
Container::Container(const Container& another) : capacity(another.capacity) , index(another.index)
{
	for(auto block : another.blockSet)
	{
		blockSet.emplace(block.getColor()->getColor());
	}
}

/**
 * Operator przypisania.
 */
Container& Container::operator=(const Container & rhs)
{
	if(this == &rhs)
		return *this;

	for(auto block : rhs.blockSet)
	{
		blockSet.emplace(block.getColor()->getColor());
	}
	this->capacity = rhs.capacity;
	this->index = rhs.index;
	return *this;
}

/**
 * Przeniesienie klocka o kolorze blockColor do pojemnika destiny.
 * False zwracany jesli :
 * - klocka o danym kolorze nie ma w pojemniku this.
 * - klocek nie moze zostac przeniesiony do destiny, poniewaz nie ma tam miejsca.
 *
 */
bool Container::moveBlock(const Color & color, Container& destiny)
{
	Block block(color.getColor());

	// nie ma co przelozyc.
	if(getColorMultiplicity(color) == 0)
	{
		return false;
	}
	// nie ma miejsca w destiny
 	if(destiny.getLeftPlace() == 0)
 	{
		return false;
 	}
	blockSet.erase(blockSet.find(block));
	destiny.blockSet.emplace(block.getColor());
	return true;
}

/**
 * Destruktor.
 */
Container::~Container(){ }

/**
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

/**
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

/**
 * Pobranie pojemnosci pojemnika.
 */
unsigned int Container::getCapacity() const
{
	return capacity;
}

/**
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
Color* Container::checkIsColorPresent(const std::map<Color, bool, ColorCompare> & colorOrganizeMap) const
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

/**
 * Pobiera kolor pierwszego z brzegu klocka.
 */
Color* Container::getTopColor() const
{
	std::multiset<Block, BlockCompare>::iterator iter = blockSet.begin();
	if(iter == blockSet.end())
		return nullptr;
	return iter->getColor();
}

/**
 * Pobiera pierszy kolor z brzegu, ale z wyjatkiem klocka o kolorze anotherColor.
 * Jesli w pojemniku jest >1 klockow o kolorze another Color, to moze zostac zwrocony anotherColor.
 */
Color* Container::getTopColor(const Color * anotherColor) const
{
	for (auto iter = blockSet.begin(); iter != blockSet.end(); ++iter)
	{
		if (iter->getColor()->getColor() != anotherColor->getColor())
		{
			return iter->getColor();
		}
	}

	if (getColorMultiplicity(*anotherColor) > 1)
	{
		for (auto iter = blockSet.begin(); iter != blockSet.end(); ++iter)
		{
			if (iter->getColor()->getColor() == anotherColor->getColor())
			{
				return iter->getColor();
			}
		}
	}

	return nullptr;
}

/**
 * Zapisanie informacji o pojemniku do pliku.
 */
void Container::showInfoToFile( std::ofstream & os ) const
{
	os << "Index : " << index << " capacity : " << capacity << " leftPlace : " << getLeftPlace( ) << " state : " << this->checkState( ) << "\n";
	for ( unsigned int i = 0; i < Color::getNumberOfAllColors( ); ++i ) {
		os << i << " : " << blockSet.count( Block( i ) ) << "\n";
	}
}