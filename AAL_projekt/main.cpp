#include <iostream>
#include "Interpreter.h"
#include "ContainerSet.h"
#include "CycleCounter.h"

/* Naglowki plikow z algorytmami rozwiazan */
#include "ContainersSweep.h"

/*
 * Wejscie do programu.
 */
int main(int argc, char* argv[])
{
	Interpreter interpreter(argv[1]);
	CycleCounter cycleCounter;
	
	if (interpreter.verifyGatheredInfo())
	{
		interpreter.showGatheredInfo();
	}
	
	ContainerSet containerSet(interpreter.getConatinersCapacity(),
							  interpreter.getIncludingList(),
							  interpreter.getColorsNumber());

	containerSet.showInfo();

	ContainersSweep containersSweep(containerSet);

	cycleCounter.startCounting();
	containersSweep.solveProblem();
	cycleCounter.stopCounting();

	std::cout << cycleCounter.getNumerOfCycles() << std::endl;

	system("PAUSE");
	return 0;
}