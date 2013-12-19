#ifndef CONTAINER_H
#define CONTAINER_H

#include <iostream>
#include <unordered_map>
#include <vector>

/*
 * Klasa pojemnika.
 */
class Container
{
private:
	//Mapuje kolor na ilosc przechowywanych klockow w tym kolorze.
	std::unordered_map<unsigned int, unsigned int> content;
	//Pojemnosc pojmenika.
	const unsigned int capacity;
	//Pozostale miejsce do zajecia.
	unsigned int leftPlace;
public:
	Container(const std::vector<unsigned int> & included,
		      const unsigned int capacity,
			  const unsigned int colorsNumber);
	~Container();
	Container(const Container & another);
	bool moveBlock(unsigned int blockColor, Container& destiny);
	void showInfo() const;
	bool checkState() const;
	unsigned int getCapacity() const;
	unsigned int getLeftPlace() const;
	unsigned int getColorMultiplicity(unsigned int color) const;
	long checkIsColorPresent(std::vector<bool> & table) const;
};

#endif 
