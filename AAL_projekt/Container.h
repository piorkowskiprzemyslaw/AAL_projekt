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
	//Pojemno�� pojmenika.
	const unsigned int capacity;
	//Pozosta�e miejsce do zaj�cia.
	unsigned int leftPlace;
public:
	Container(const std::vector<unsigned int> & included,
		      const unsigned int capacity,
			  const unsigned int colorsNumber);
	~Container();
	Container(const Container & anther);
	bool moveBlock(unsigned int blockColor, Container& destiny);
	void showInfo() const;
	bool checkState() const;
	unsigned int getCapacity() const;
};

#endif 