#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>

/*
 * Blok o zadanym w konstruktorze kolorze.
 */
class Block
{
private:
	const unsigned int color;
public:
	Block(unsigned int color);
	~Block();
	unsigned int getColor();
};

#endif BLOCK_H