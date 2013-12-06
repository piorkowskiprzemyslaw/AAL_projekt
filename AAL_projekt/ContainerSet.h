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
	// Wektor kontenerów znajduj¹cych siê w tym zbiorze.
	std::vector<Container *> containers;
	//liczba kolorów
	unsigned int colorsNumber;
	//liczba pojemników.
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
