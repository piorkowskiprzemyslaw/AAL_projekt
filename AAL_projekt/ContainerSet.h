#ifndef CONTAINERSET_H
#define CONTAINERSET_H

#include <iostream>
#include <vector>
#include "Container.h"

/*
 * Zbiór pojemników.
 */
class ContainerSet
{
private:
	// Tablica kontenerów.
	Container * * containers;
	//liczba pojemników.
	size_t arraySize;
	//liczba kolorów
	unsigned int colorsNumber;
	//suma pojemnosci wszystkich pojemników.
	size_t capacitySum;

public:
	ContainerSet(const std::vector<unsigned int> & containersCapacity,
		         const std::vector<std::vector<unsigned int> > & includingList,
				 const unsigned int colorsNumber);
	~ContainerSet();
	explicit ContainerSet(const ContainerSet& another);

	//Klasa iteratora do przechodzenia po zbiorze pude³ek
	class const_iterator
	{
	protected:
		size_t index;
		Container * containerWsk;
		Container * * containerArrayWsk;
		const size_t arraySize;
	public:
		const_iterator() : arraySize(0) {}
		const_iterator(const const_iterator & a) : index(a.index), containerWsk(a.containerWsk), containerArrayWsk(a.containerArrayWsk), arraySize(arraySize) {}
		const_iterator(const size_t ind, Container * * array, const size_t arraySize) : arraySize(arraySize) { index = ind; containerArrayWsk = array; containerWsk = containerArrayWsk[index]; }
		const_iterator(Container * wsk, size_t ind, Container * * array, const size_t arraySize) : index(ind), containerWsk(wsk), containerArrayWsk(array), arraySize(arraySize) {}

		inline const Container * operator->() const { return containerWsk; }
		inline const Container * operator*()  const { return containerWsk; }

		const_iterator& operator++()
		{
			if (++index >= arraySize)
				index = 0;
			containerWsk = containerArrayWsk[index];
			return *this;
		}

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
	//Klasa zwyk³ego iteratora.
	class iterator : public const_iterator
	{
	protected:
	public:
		iterator() : const_iterator() {}
		iterator(const const_iterator & a) : const_iterator(a) {}
		iterator(const size_t ind, Container * * array, const size_t arraySize) : const_iterator(ind, array, arraySize) { }
		iterator(Container * wsk, size_t ind, Container * * array, const size_t arraySize) : const_iterator(wsk, ind, array, arraySize) {}

		inline Container* operator*()  const { return this->containerWsk; }
		inline Container* operator->() const { return this->containerWsk; }

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

	iterator begin();
	const_iterator begin() const;
	//iterator end();
	//const_iterator end() const;
};

#endif
