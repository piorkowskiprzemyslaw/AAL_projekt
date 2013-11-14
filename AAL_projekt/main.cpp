#include <iostream>
#include "Interpreter.h"

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

	system("PAUSE");
	return 0;
}