#ifndef CONTAINERSSWEEP_H
#define CONTAINERSSWEEP_H

#include <iostream>
#include "ContainerSet.h"

/*
 * Klasa realizuj¹ca podejœcie do rozwiazania problemu za pomoc¹ omiatania pojemników z klocków
 * o kolorach ktore ju¿ nie mog¹ siê w tym pojemniku znajdowaæ.
 */
template <int (*sort)(std::pair<unsigned int, unsigned int> * array, unsigned int size) >
class ContainersSweep
{
private:
	ContainerSet * containerSet;
	size_t * colorMultiplicity;
	bool checkPreconditions();
public:
	ContainersSweep();
	ContainersSweep(const ContainerSet & containerSet);
	~ContainersSweep();
	void solveProblem();
};

#endif

