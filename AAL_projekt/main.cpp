#include <iostream>
#include "Interpreter.h"
#include "ContainerSet.h"

/*
 * Wejscie do programu.
 */
int main(int argc, char* argv[])
{
	Interpreter interpreter(argv[1]);
	
	if (interpreter.verifyGatheredInfo())
	{
		interpreter.showGatheredInfo();
	}
	
	ContainerSet containerSet(interpreter.getConatinersCapacity(),
							  interpreter.getIncludingList(),
							  interpreter.getColorsNumber());

	containerSet.showInfo();

	system("PAUSE");
	return 0;
}