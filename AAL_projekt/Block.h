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
	Color * color;
public:
	Block(unsigned int colorNo);
	Block(Color * color);
	Block(const Block & another);
	Block(const Color & color);
	~Block();
	Block& operator=(const Block& rhs);
	bool operator==(const Block & rhs) const;
	bool operator<(const Block & rhs) const;
	Color* getColor() const;
};

#endif
