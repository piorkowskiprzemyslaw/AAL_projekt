#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <vector>
#include <fstream>

/*
 * Klasa odpowiedzialna za odczyt przekazywanych informacji z pliku.
 */
class Interpreter
{
private:
	unsigned int containersNumber;
	std::vector<unsigned int> containersCapacity;
	unsigned int colorsNumber;
	std::vector<unsigned int> colorsMultiplicity;
	std::vector<std::vector<unsigned int>> includingList;
public:
	Interpreter(char* fileName);
	~Interpreter();
	unsigned int getContainersNumber();
	std::vector<unsigned int> & getConatinersCapacity();
	unsigned int getColorsNumber();
	std::vector<unsigned int> & getColorsMultiplicity();
	std::vector<std::vector<unsigned int>> & getIncludingList();
	void showGatheredInfo();
	bool verifyGatheredInfo();
};

#endif