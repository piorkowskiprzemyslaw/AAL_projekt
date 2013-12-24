#ifndef CONTAINERSET_H
#define CONTAINERSET_H

#include <iostream>
#include <vector>
#include "Container.h"
#include "Color.h"
#include "cmath"

enum Direction {
	left = 0,
	right = 1
};

/**
 * Zbior pojemnikow z wazna relacja sasiedztwa konkretnrych pojemnikow.
 */
class ContainerSet
{
private:
	// Tablica kontenerow.
	std::vector<Container> containers;
	//suma pojemnosci wszystkich pojemnikow.
	size_t capacitySum;

public:
	ContainerSet(const std::vector<unsigned int> & containersCapacity, const std::vector<std::vector<unsigned int> > & includingList, const unsigned int colorsNumber);
	~ContainerSet();
	explicit ContainerSet(const ContainerSet& another);

	//Klasa iteratora do przechodzenia po zbiorze pudelek
	class const_iterator
	{
	protected:
		std::vector<Container> & container;
		std::vector<Container>::iterator iter;
	public:
		const_iterator(const const_iterator & a) : container(a.container) , iter(a.iter) {}
		const_iterator(std::vector<Container> & cont, const size_t pos) : container(cont)
		{
			iter = cont.begin() + pos;
			container = cont;
		}

		inline const Container * operator->() const { return iter.operator ->(); }
		inline const Container& operator*()  const { return iter.operator *(); }

		// przejscie w prawo
		const_iterator& operator++()
		{
			++iter;
			if ( iter == container.end())
				iter = container.begin();
			return *this;
		}

		const_iterator operator++(int)
		{
			const_iterator backup(*this);
			this->operator++();
			return backup;
		}

		// przejscie w lewo
		const_iterator& operator--()
		{
			if(iter == container.begin())
			{
				iter = container.end();
				--iter;
			} else {
				--iter;
			}
			return *this;

		}

		const_iterator operator--(int)
		{
			const_iterator backup(*this);
			this->operator--();
			return backup;
		}
	};

	//Klasa zwyklego iteratora.
	class iterator : public const_iterator
	{
	public:
		iterator(const const_iterator & a) : const_iterator(a) {}
		iterator(std::vector<Container> & cont, const size_t pos) : const_iterator(cont,pos) {}

		inline Container& operator*() const { return this->iter.operator *(); }
		inline Container* operator->() const { return this->iter.operator ->(); }
		inline bool operator==(const iterator & another) { return this->iter == another.iter; }
		inline bool operator!=(const iterator & another) { return this->iter != another.iter; }

		iterator& operator=(const iterator & another)
		{
			if(*this == another)
				return *this;
			this->iter = another.iter;
			this->container = another.container;
			return *this;
		}

		iterator& operator++()
		{
			++(*(const_iterator*)this);
			return (*this);
		}

		iterator operator++(int)
		{
			iterator temp = *this;
			++*this;
			return temp;
		}

		iterator& operator--()
		{
			--(*(const_iterator*)this);
			return (*this);
		}

		iterator operator--(int)
		{
			iterator temp = *this;
			--*this;
			return temp;
		}

	};

	void showInfo() const;
	unsigned int getColorsNumber() const;
	unsigned int colorMultiplicity(const Color & color) const;
	size_t size() const;
	size_t getCapacitySum() const;
	iterator getMaxiumWithColor(const Color & color);
	iterator begin();
	unsigned int swapBlocksWithFreeSpace(iterator & freeSpace, iterator & firstBlockLocation, Color & firstColor, iterator & secondBlockLocation, Color & secondColor);

private:
	void updateShiftVector(std::vector<Color *> & vector, const iterator & freeSpace, const Direction & dir);
	unsigned int getDistance(Direction dir, iterator & first, iterator & second) const;
	unsigned int shiftBlocks(std::vector<Color *> & shfitVector, const iterator & freeSpace, unsigned int distance, Direction dir);

};


#endif
