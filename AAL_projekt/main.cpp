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
		interpreter.showGatheredInfo();
	}

	ContainerSet containerSet(interpreter.getConatinersCapacity(), interpreter.getIncludingList(), interpreter.getColorsNumber());
	containerSet.showInfo();

	ContainerSet::iterator firstBlock = containerSet.begin();
	firstBlock++;
	ContainerSet::iterator secondBlock = containerSet.begin();
	secondBlock++;
	secondBlock++;
	secondBlock++;
	secondBlock++;
	Color col1(1);
	Color col2(4);

	containerSet.swapBlocksWithFreeSpace(firstBlock, firstBlock, col1, secondBlock, col2);

	//ContainersSweep<SelectionSort> sweep(containerSet);

	//cycleCounter.startCounting();
    //sweep.solveProblem();
    //cycleCounter.startCounting();

    //ContainerSet containerSet1(generator.getConatinersCapacity(), generator.getIncludingList(), generator.getColorsNumber());
	//containerSet1.showInfo();


    std::cout << std::endl << "Number of cycles : " << cycleCounter.getNumerOfCycles() << std::endl;

    #ifdef _WIN32
	system("PAUSE");
	#endif

	return 0;
}

