#include "Interpreter.h"

/*
 * Konsturktor przyjmuj¹cy nazwe pliku z którego ma czytaæ.
 */
Interpreter::Interpreter(char* fileName)
{
	int tmpVal;
	int actualIndex;
	std::ifstream inputFile;
	inputFile.open(fileName);
	if (!inputFile.good())
	{
		std::cout << "File opening error!" << fileName << std::endl;
		exit(1);
	}

	inputFile >> containersNumber;

	for (size_t i = 0; i < containersNumber; i++)
	{
		inputFile >> tmpVal;
		containersCapacity.push_back(tmpVal);
	}

	inputFile >> colorsNumber;

	for (size_t i = 0; i < colorsNumber; i++)
	{
		inputFile >> tmpVal;
		colorsMultiplicity.push_back(tmpVal);
	}

	for (size_t i = 0; i < containersNumber; i++)
	{
		includingList.push_back(std::vector<unsigned int>());
	}

	while (inputFile >> actualIndex)
	{
		while (inputFile.peek() != '\n')
		{
			inputFile >> tmpVal;
			includingList[actualIndex - 1].push_back(tmpVal - 1);
		}
	}

	inputFile.close();
}

/*
 * Destruktor
 */
Interpreter::~Interpreter()
{
}

/*
 * Pobranie liczby pojemników.
 */
unsigned int Interpreter::getContainersNumber()
{
	return containersNumber;
}

/*
 * Pobranie pojemnoœci pojemników.
 */
std::vector<unsigned int> & Interpreter::getConatinersCapacity()
{
	return containersCapacity;
}

/*
 * Pobranie iloœci kolorów.
 */
unsigned int Interpreter::getColorsNumber()
{
	return colorsNumber;
}

/*
 * Pobranie licznoœci ka¿dego z kolorów.
 */
std::vector<unsigned int> & Interpreter::getColorsMultiplicity()
{
	return colorsMultiplicity;
}

/*
 * Pobranie tablic zawierania.
 */
std::vector<std::vector<unsigned int>> & Interpreter::getIncludingList()
{
	return includingList;
}

/*
 * Wyswietlenie pobranych informacji - glownie cele debuggowania.
 */
void Interpreter::showGatheredInfo()
{
	std::cout << "Number of containers : " << containersNumber << std::endl;
	for (size_t i = 0; i < containersCapacity.size(); i++)
	{
		std::cout << containersCapacity[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "Number of colors : " << colorsNumber << std::endl;
	for (size_t i = 0; i < colorsMultiplicity.size(); i++)
	{
		std::cout << colorsMultiplicity[i] << " ";
	}
	std::cout << std::endl;

	for (size_t i = 0; i < includingList.size(); i++)
	{
		std::cout << "Colors inside container " << i << std::endl;
		for (size_t j = 0; j < includingList[i].size(); j++)
		{
			std::cout << includingList[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

/*
 * Weryfikacja sensu pobranych danych.
 */
bool Interpreter::verifyGatheredInfo()
{
	std::vector<unsigned int> colorsCopy(colorsMultiplicity);
	if (includingList.size() > containersNumber)
	{//sprawdzenie czy nie probowano wlozyc klockow do wiekszej ilosci pojemnikow.
		std::cout << "Proba wlozenia klockow do za duzej ilosci pojemnikow!" << std::endl;
		return false;
	}

	for (size_t i = 0; i < includingList.size(); i++)
	{
		if (includingList[i].size() > containersCapacity[i])
		{//sprawdznie czy do pojemnika nie zostalo wlozone wiecej niz on moze zmiescic.
			std::cout << "Proba wlozenia za duzej ilosci klockow do pojemnika!" << std::endl;
			return false;
		}

		for (size_t j = 0; j < includingList[i].size(); j++)
		{
			if (includingList[i][j] > colorsNumber)
			{//sprawdzenie czy nie zostalo wlozone za duzo kolorow.
				std::cout << "Proba wlozenia nie prawidlowego koloru!" << std::endl;
				return false;
			}

			colorsCopy[includingList[i][j]] -= 1;
		}
	}

	for (size_t i = 0; i < colorsCopy.size(); i++)
	{
		if (colorsCopy[i] != 0)
		{
			std::cout << "Nie rozlozono wszystkich klockow!" << std::endl;
			return false;
		}
	}

	return true;
}