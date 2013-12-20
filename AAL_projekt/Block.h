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
	Block(const Block & another);
	~Block();
	Block& operator=(const Block& rhs);
	bool operator==(const Block & rhs) const;
	bool operator<(const Block & rhs) const;
	Color* getColor() const;
};

#endif
