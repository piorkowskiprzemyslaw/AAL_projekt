#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>
#include <memory>
#include "Color.h"

/*
 * Blok o zadanym w konstruktorze kolorze.
 */
class Block
{
private:
	std::unique_ptr<Color> color;
public:
	Block(unsigned int colorNo);
	Block(std::unique_ptr<Color> color);
	~Block();
	bool operator==(const Block & rhs);
	Color* getColor() const;
};

#endif BLOCK_H