#ifndef CONTAINER_H
#define CONTAINER_H

#include <iostream>
#include <unordered_map>
#include <set>
#include <vector>
#include <cassert>
#include <iterator>
#include "Block.h"
#include "BlockCompare.h"
#include "Color.h"

/*
 * Klasa pojemnika.
 */
class Container
{
private:
	//Zbior przechowywanych klcokow.
	std::multiset<Block,BlockCompare> blockSet;
	//Pojemnosc pojmenika.
	unsigned int capacity;
	//Numer pojemnika
	unsigned int index;
public:
	Container();
	Container(const std::vector<unsigned int> & included,
		      const unsigned int capacity, const unsigned int index);
	~Container();
	Container(const Container & another);
	Container& operator=(const Container & rhs);
	bool moveBlock(unsigned int blockColor, Container& destiny);
	void showInfo() const;
	bool checkState() const;
	unsigned int getCapacity() const;
	unsigned int getLeftPlace() const;
	unsigned int getColorMultiplicity(unsigned int color) const;
	long checkIsColorPresent(std::vector<bool> & table) const;
	unsigned int getIndex() const;
};

#endif
