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
	Container(const std::vector<unsigned int> & included, const unsigned int capacity, const unsigned int index);
	~Container();
	Container(const Container & another);
	Container& operator=(const Container & rhs);
	bool moveBlock(const Color & color, Container& destiny);
	unsigned int getColorMultiplicity(const Color & color) const;
	Color* checkIsColorPresent(const std::map<Color, bool, ColorCompare> & colorOrganizeMap) const;
	unsigned int getCapacity() const;
	unsigned int getLeftPlace() const;
	void showInfo() const;
	bool checkState() const;
	Color* getTopColor() const;
	unsigned int getIndex() const;
};

#endif
