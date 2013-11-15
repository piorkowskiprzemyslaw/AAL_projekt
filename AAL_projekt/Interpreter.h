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
	//Liczba pojemników.
	unsigned int containersNumber;
	//Pojemnosci kazdego z pojemników.
	std::vector<unsigned int> containersCapacity;
	//Wektor rozró¿nialnych kolorów.
	unsigned int colorsNumber;
	//Liczba kloców w ka¿dym kolorze.
	std::vector<unsigned int> colorsMultiplicity;
	//Wektor wektorów z zawieranymi kolorami w ramach ka¿dego pojemnika.
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