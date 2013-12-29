/*
 * Projekt z AAL.
 *
 * Przemyslaw Piorkowski.
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
	DataGenerator generator(6, 6, 6, 0.95f, 6);
	generator.generateData();
	CycleCounter cycleCounter;

	if (interpreter.verifyGatheredInfo())
	{
		interpreter.showGatheredInfo();
	}

	//ContainerSet containerSet(generator.getConatinersCapacity(), generator.getIncludingList(), generator.getColorsNumber());
	ContainerSet containerSet( interpreter.getConatinersCapacity( ), interpreter.getIncludingList( ), interpreter.getColorsNumber( ) );
	//containerSet.showToFile();
	containerSet.showInfo( );
	ContainersSweep<SelectionSort> sweep( containerSet );

	cycleCounter.startCounting( );
	sweep.solveProblem( );
	cycleCounter.stopCounting( );

	std::cout << std::endl << "After solving with "<< sweep.getCounter() << " moves ... " << std::endl;
	sweep.showContainerSet( );

    std::cout << std::endl << "Number of cycles : " << cycleCounter.getNumerOfCycles() << std::endl;


	//ContainerSet::iterator firstBlock = containerSet.begin();
	//++firstBlock;
	//++firstBlock;
	//ContainerSet::iterator secondBlock = containerSet.begin();
	//++secondBlock;
	//++secondBlock;
	//++secondBlock;
	//++secondBlock;
	//ContainerSet::iterator freeSpace = containerSet.begin();
	//freeSpace++;
	//freeSpace++;
	//freeSpace++;
	//freeSpace++;
	//freeSpace++;
	//Color col1(3);
	//Color col2(4);

	//containerSet.swapBlocksWithFreeSpace( freeSpace, firstBlock, col1, secondBlock, col2 );
	///containerSet.showInfo( );
	//ContainersSweep<SelectionSort> sweep(containerSet);

	//cycleCounter.startCounting();
    //sweep.solveProblem();
    //cycleCounter.startCounting();

    //ContainerSet containerSet1(generator.getConatinersCapacity(), generator.getIncludingList(), generator.getColorsNumber());
	//containerSet1.showInfo();


    #ifdef _WIN32
	system("PAUSE");
	#endif

	return 0;
}

