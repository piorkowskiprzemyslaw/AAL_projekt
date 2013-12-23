#ifndef CONTAINER_H
#define CONTAINER_H

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <cassert>
#include <iterator>
#include "Block.h"
#include "BlockCompare.h"
#include "Color.h"
#include "ColorCompare.h"

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
	bool moveBlock(Color & color, Container& destiny);
	void showInfo() const;
	bool checkState() const;
	unsigned int getCapacity() const;
	unsigned int getLeftPlace() const;
	unsigned int getColorMultiplicity(unsigned int color) const;
	unsigned int getColorMultiplicity(Color & color) const;
	Color * checkIsColorPresent(std::map<Color, bool, ColorCompare> & colorOrganizeMap) const;
	unsigned int getIndex() const;
	Color* getTopColor() const;
};

#endif
