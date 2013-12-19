#include "Block.h"

/*
 * Konstruktor pobierajacy kolor tworzonego klocka.
 */
Block::Block(unsigned int color) 
{
	this->color = std::unique_ptr<Color>(new Color(color));
}

/*
 * Zainicjalizowanie istniejacym unique_ptr'em 
 */
Block::Block(std::unique_ptr<Color> color)
{
	this->color = std::move(color);
}

/*
 * Dekstrutkor.
 */
Block::~Block() { }

/*
 * Operator porownania dwoch klockow. Sa takie same jesli maja takie same kolory.
 */
bool Block::operator==(const Block & rhs)
{
	return *(color.get()) == *(rhs.color.get());
}

/*
 * Zwraca wskaznik na kolor tego klocka
 */
Color* Block::getColor() const
{
	return this->color.get();
}
