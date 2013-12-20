#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>

/*
 * Klasa odpowiedzialna za odczyt przekazywanych informacji z pliku.
 */
class Interpreter
{
private:
	//Liczba pojemnikow.
	unsigned int containersNumber;
	//Pojemnosci kazdego z pojemnikow.
	std::vector<unsigned int> containersCapacity;
	//Wektor rozronialnych kolorow.
	unsigned int colorsNumber;
	//Liczba klocow w kazdym kolorze.
	std::vector<unsigned int> colorsMultiplicity;
	//Wektor wektorow z zawieranymi kolorami w ramach kazdego pojemnika.
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
