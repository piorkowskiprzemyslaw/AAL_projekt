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

/*
 * Metoda rozwiazujaca problem implementowanym algorytmem.
 */
void ContainersSweep::solveProblem()
{

}
