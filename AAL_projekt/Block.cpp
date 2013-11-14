#include "Block.h"

/*
 * Konstruktor pobierajacy kolor tworzonego klocka.
 */
Block::Block(unsigned int color = 0) : color(color){ }

/*
 * Dekstrutkor.
 */
Block::~Block() { }

/*
 * Pobranie koloru klocka.
 */
unsigned int Block::getColor()
{
	return color;
}