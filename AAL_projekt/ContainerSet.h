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
public:
	ContainerSet(const std::vector<unsigned int> & containersCapacity,
		         const std::vector<std::vector<unsigned int> > & includingList,
				 const unsigned int colorsNumber);
	~ContainerSet();
	void showInfo();
};

#endif