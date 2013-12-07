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
	unsigned int allColorSum = 0;
	unsigned int allCapacitySum = 0;

	//liczba kolorow musi byc mniejsza badz rowna liczbe dostepnych pojemnikow.
	if (this->containerSet->getColorsNumber() > this->containerSet->size())
	{
		return false;
	}

    //lacznie we wszystkich pojemnikach jest nie wiecej niz n klockow danego koloru.
	for(int i = 0 ; i < containerSet->getColorsNumber() ; i++)
	{
		allColorSum += containerSet->colorMultiplicity(i);
		if (containerSet->colorMultiplicity(i) > containerSet->size())
		{
			return false;
		}

	}

	//zapewnienie aby wszystkie klocki miescily sie w pojemnikach, oraz ¿eby da³o
	//siê wykonaæ ruch.
	if (allColorSum >= containerSet->getCapacitySum())
	{
		return true;
	}

	return true;
}

/*
 * Metoda rozwiazujaca problem implementowanym algorytmem.
 */
void ContainersSweep::solveProblem()
{
	std::cout << "Solving problem with ContainersSweep" << std::endl;
	std::cout << "Preconditions ... " << std::boolalpha << checkPreconditions() << std::noboolalpha << std::endl;


}
