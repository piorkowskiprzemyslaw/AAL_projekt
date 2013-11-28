#include "ContainersSweep.h"

/*
 * Domyslny konstruktor.
 */
ContainersSweep::ContainersSweep()
{
	containerSet = nullptr;
}


/*
 * Konstrutkor pobierajacy konkretny zbiór pojemników.
 */
ContainersSweep::ContainersSweep(const ContainerSet & containerSet)
{
	this->containerSet = new ContainerSet(containerSet);
}

/*
 * Destruktor.
 */
ContainersSweep::~ContainersSweep()
{
	delete(containerSet);
}

bool ContainersSweep::checkPreconditions()
{
	return false;
}

/*
 * Metoda rozwiazujaca problem implementowanym algorytmem.
 */
void ContainersSweep::solveProblem()
{
	std::cout << "Solving problem with ContainersSweep" << std::endl << std::endl;
}
