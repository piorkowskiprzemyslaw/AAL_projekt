#include <iostream>
#include "Interpreter.h"
#include "ContainerSet.h"
#include "CycleCounter.h"
#include "DataGenerator.h"

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
		//interpreter.showGatheredInfo();
	}
	
	ContainerSet containerSet(interpreter.getConatinersCapacity(), interpreter.getIncludingList(), interpreter.getColorsNumber());

	//containerSet.showInfo();

	DataGenerator generator(4, 1, 5, 0.75, 4);

	generator.generateData();
	generator.showGeneratedInfo();

	ContainerSet containerSet1(generator.getConatinersCapacity(), generator.getIncludingList(), generator.getColorsNumber());
	containerSet1.showInfo();

	system("PAUSE");
	return 0;
}