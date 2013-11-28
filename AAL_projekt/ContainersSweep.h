#ifndef CONTAINERSSWEEP_H
#define CONTAINERSSWEEP_H

#include <iostream>
#include "ContainerSet.h"

/*
 * Klasa realizuj¹ca podejœcie do rozwiazania problemu za pomoc¹ omiatania pojemników z klocków
 * o kolorach ktore ju¿ nie mog¹ siê w tym pojemniku znajdowaæ.
 */
class ContainersSweep
{
private:
	ContainerSet * containerSet;
	bool checkPreconditions();
public:
	ContainersSweep();
	ContainersSweep(const ContainerSet & containerSet);
	~ContainersSweep();
	void solveProblem();
};

#endif

