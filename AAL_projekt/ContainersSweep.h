#ifndef CONTAINERSSWEEP_H
#define CONTAINERSSWEEP_H

#include <iostream>
#include "ContainerSet.h"

/*
 * Klasa realizuj�ca podej�cie do rozwiazania problemu za pomoc� omiatania pojemnik�w z klock�w
 * o kolorach ktore ju� nie mog� si� w tym pojemniku znajdowa�.
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

