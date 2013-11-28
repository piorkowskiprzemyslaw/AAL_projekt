#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <random>

/*
 * Klasa przedstawiaj¹ca generator danych dla problemu.
 * Wymagane jest podanie : 
 * - ilosc pojemnikow
 * - maksymalna pojemnosc pojemnika
 * - minimalna pojemnosc pojemnika
 * - stopien zapelnienia pojemnikow
 * - liczba klockow
 *
 */
class DataGenerator
{
private:
	//ilosc pojemnikow
	const unsigned int numberOfContainers;
	//stopien zapelnienia pojemnikow
	const float fillingDegree;
	//liczba kolorów
	const unsigned int colorsNumber;
	//maksymalna pojemnosc pojemnika
	const unsigned int minCapacity;
	//maksymalna pojemnosc pojemnika
	const unsigned int maxCapacity;
	//pojemnosci pojemników
	std::vector<unsigned int> containersCapacity;
	//lista zawierania
	std::vector<std::vector<unsigned int> > includingList;
	//generator liczb pseudolosowych
	std::default_random_engine engine;
public:
	DataGenerator(const unsigned int numberOfContainers, const unsigned int minCapacity, const unsigned int maxCapacity, const float fillingDegree, const unsigned int colorsNumber);
	void generateData();
	~DataGenerator();
	std::vector<unsigned int> & getConatinersCapacity();
	std::vector<std::vector<unsigned int>> & getIncludingList();
	unsigned int getColorsNumber();
	void showGeneratedInfo();
};

#endif

