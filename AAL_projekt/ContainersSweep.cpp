#include "ContainersSweep.h"

/*
 * Domyslny konstruktor.
 */
template <int (*sort)(std::pair<unsigned int, unsigned int> * array, unsigned int size) >
ContainersSweep<sort>::ContainersSweep()
{
	containerSet = nullptr;
	colorMultiplicity = nullptr;
}


/*
 * Konstrutkor pobierajacy konkretny zbiór pojemników.
 */
template <int (*sort)(std::pair<unsigned int, unsigned int> * array, unsigned int size) >
ContainersSweep<sort>::ContainersSweep(const ContainerSet & containerSet)
{
	this->containerSet = new ContainerSet(containerSet);
	colorMultiplicity = new size_t[this->containerSet->getColorsNumber()];
}

/*
 * Destruktor.
 */
template <int (*sort)(std::pair<unsigned int, unsigned int> * array, unsigned int size) >
ContainersSweep<sort>::~ContainersSweep()
{
	delete(containerSet);
	delete(colorMultiplicity);
}

/*
 * Sprawdz warunki poczatkowe.
 */
template <int (*sort)(std::pair<unsigned int, unsigned int> * array, unsigned int size) >
bool ContainersSweep<sort>::checkPreconditions()
{
	unsigned int allColorSum = 0;
	unsigned int allCapacitySum = 0;

	//liczba kolorow musi byc mniejsza badz rowna liczbe dostepnych pojemnikow.
	if (this->containerSet->getColorsNumber() > this->containerSet->size())
	{
		return false;
	}

    //lacznie we wszystkich pojemnikach jest nie wiecej niz n klockow danego koloru.
    //przy okazji wykonuje zliczanie każdego koloru - potrzebne do dalszej czesci algorytmu,
	for(int i = 0 ; i < containerSet->getColorsNumber() ; i++)
	{
		colorMultiplicity[i] = containerSet->colorMultiplicity(i);
		allColorSum += colorMultiplicity[i];
		if (colorMultiplicity[i] > containerSet->size())
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
template <int (*sort)(std::pair<unsigned int, unsigned int> * array, unsigned int size) >
void ContainersSweep<sort>::solveProblem()
{
	std::cout << "Solving problem with ContainersSweep" << std::endl;
	std::cout << "Checking preconditions ... ";

	if(!checkPreconditions())
	{
        std::cout << "[FALSE]" << std::endl;
        std::cout << "End of solving problem" << std::endl;
        return;
	}

	std::cout << "[OK]" << std::endl;

	std::cout << "Colors multiplicity  : " << std::endl;
	for(int i = 0 ; i < containerSet->getColorsNumber() ; ++i)
	{
        std::cout << "Color [" << i << "] = " << colorMultiplicity[i] << std::endl;
	}

    //zliczanie wystapien poszczególnych kolorów we wszystkich pudełkach.

}
