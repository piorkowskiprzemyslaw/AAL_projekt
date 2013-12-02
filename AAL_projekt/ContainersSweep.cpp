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
 * Sprawdz warunki poczatkowe.
 */
bool ContainersSweep::checkPreconditions()
{
	//liczba kolorow musi byc mniejsza badz rowna liczbe dostepnych pojemnikow.
	if (this->containerSet->getColorsNumber() > this->containerSet->getContainersNumer())
	{
		return false;
	}

	return true;

}

/*
 * Metoda rozwiazujaca problem implementowanym algorytmem.
 */
void ContainersSweep::solveProblem()
{
	std::cout << "Solving problem with ContainersSweep" << std::endl << std::endl;
}
