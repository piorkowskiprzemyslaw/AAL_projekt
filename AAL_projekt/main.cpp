/*
 * Projekt z AAL.
 *
 * Przemys³aw Piórkowski.
 */

#include <iostream>
#include <stdlib.h>
#include "Interpreter.h"
#include "ContainerSet.h"
#include "CycleCounter.h"
#include "DataGenerator.h"

/* Naglowki plikow z algorytmami rozwiazan */
#include "ContainersSweep.h"
#include "SearchTree.h"
#include "SelectionSort.h"

/*
TODO:
- operator przypisania dla iteratora w ContainerSet.
- dalsza czesc algorytmu.
*/

/*
 * Wejscie do programu.
 */
int main(int argc, char* argv[])
{
	Interpreter interpreter(argv[1]);
	DataGenerator generator(6, 3, 6, 0.85, 6);
	generator.generateData();
	CycleCounter cycleCounter;

	if (interpreter.verifyGatheredInfo())
	{
		//interpreter.showGatheredInfo();
	}

	ContainerSet containerSet(interpreter.getConatinersCapacity(), interpreter.getIncludingList(), interpreter.getColorsNumber());
	//containerSet.showInfo();
	ContainerSet containerSet1(generator.getConatinersCapacity(), generator.getIncludingList(), generator.getColorsNumber());
	containerSet1.showInfo();

	ContainersSweep<selectionSort> sweep(containerSet1);
	cycleCounter.startCounting();
    sweep.solveProblem();
    cycleCounter.startCounting();

    std::cout << std::endl << "Number of cycles : " << cycleCounter.getNumerOfCycles() << std::endl;

    #ifdef _WIN32
	system("PAUSE");
	#endif

	return 0;
}

