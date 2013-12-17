#ifndef CONTAINERSET_H
#define CONTAINERSET_H

#include <iostream>
#include <vector>
#include "Container.h"

/**
 * Zbior pojemnikow.
 */
class ContainerSet
{
private:
	// Tablica kontenerow.
	Container * * containers;
	//liczba pojemnikow.
	size_t arraySize;
	//liczba kolorow
	unsigned int colorsNumber;
	//suma pojemnosci wszystkich pojemnikow.
	size_t capacitySum;

public:
	ContainerSet(const std::vector<unsigned int> & containersCapacity,
		         const std::vector<std::vector<unsigned int> > & includingList,
				 const unsigned int colorsNumber);
	~ContainerSet();
	explicit ContainerSet(const ContainerSet& another);

	//Klasa iteratora do przechodzenia po zbiorze pudelek
	class const_iterator
	{
	protected:
		size_t arraySize;
		Container * * containerArrayWsk;
		Container * containerWsk;
		size_t index;
	public:
		const_iterator() : arraySize(0), containerArrayWsk(NULL), containerWsk(NULL), index(0) {}
		const_iterator(const const_iterator & a) :arraySize(a.arraySize), containerArrayWsk(a.containerArrayWsk),  containerWsk(a.containerWsk), index(a.index) {}
		const_iterator(const size_t ind, Container * * array, const size_t arraySize) : arraySize(arraySize) { index = ind; containerArrayWsk = array; containerWsk = containerArrayWsk[index]; }
		const_iterator(Container * wsk, size_t ind, Container * * array, const size_t arraySize) : arraySize(arraySize), containerArrayWsk(array), containerWsk(wsk), index(ind) {}

		inline const Container * operator->() const { return containerWsk; }
		inline const Container& operator*()  const { return *(containerWsk); }
		inline size_t operator()(void) const { return index;}
		const_iterator& operator=(const const_iterator& another)
		{
			if(this != &another)
			{
				arraySize = another.arraySize;
				containerArrayWsk = another.containerArrayWsk;
				containerWsk = another.containerWsk;
				index = another.index;
			}
			return *this;
		}

		// przejscie w prawo
		const_iterator& operator++()
		{
			if (++index >= arraySize)
				index = 0;
			containerWsk = containerArrayWsk[index];
			return *this;
		}

		//przejscie w lewo
		const_iterator operator++(int)
		{
			const_iterator backup(*this);
			this->operator++();
			return backup;
		}

		const_iterator& operator--()
		{
			if (--index < 0)
				index = arraySize - 1;
			containerWsk = containerArrayWsk[index];
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
		iterator() : const_iterator() {}
		iterator(const const_iterator & a) : const_iterator(a) {}
		iterator(const size_t ind, Container * * array, const size_t arraySize) : const_iterator(ind, array, arraySize) { }
		iterator(Container * wsk, size_t ind, Container * * array, const size_t arraySize) : const_iterator(wsk, ind, array, arraySize) {}

		inline Container& operator*()  const { return *(this->containerWsk); }
		inline Container* operator->() const { return this->containerWsk; }
		inline size_t operator() (void) { return ((const_iterator*)this)->operator ()(); }
		iterator& operator=(const iterator & another)
		{
			index = another.index;
			containerArrayWsk = another.containerArrayWsk;
			containerWsk = another.containerWsk;
			arraySize = another.arraySize;
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

	void showInfo();
	unsigned int getColorsNumber();
	unsigned int colorMultiplicity(unsigned int color);
	size_t size() const;
	size_t getCapacitySum() const;
	iterator getMaxiumWithColor(unsigned int color, int(*T)(std::pair<unsigned int, unsigned int> * , unsigned int)) const;

	iterator begin();
	const_iterator begin() const;
	//iterator end();
	//const_iterator end() const;
};

#endif
