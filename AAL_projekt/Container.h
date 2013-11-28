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
	//Pojemnoœæ pojmenika.
	const unsigned int capacity;
	//Pozosta³e miejsce do zajêcia.
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