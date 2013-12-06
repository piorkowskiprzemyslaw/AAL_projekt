#ifndef CONTAINERSET_H
#define CONTAINERSET_H

#include <iostream>
#include <vector>
#include "Container.h"

/*
 * Zbi�r pojemnik�w.
 */
class ContainerSet
{
private:
	// Wektor kontener�w znajduj�cych si� w tym zbiorze.
	std::vector<Container *> containers;
	//liczba kolor�w
	unsigned int colorsNumber;
	//liczba pojemnik�w.
	unsigned int containersNumber;
public:
	ContainerSet(const std::vector<unsigned int> & containersCapacity,
		         const std::vector<std::vector<unsigned int> > & includingList,
				 const unsigned int colorsNumber);
	ContainerSet(const ContainerSet& another);
	~ContainerSet();
	void showInfo();
	unsigned int getColorsNumber();
	unsigned int getContainersNumer();
	unsigned int getColorMultiplicity(unsigned int color);

};

#endif
