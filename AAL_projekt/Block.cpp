#include "Block.h"

/**
 * Konstruktor pobierajacy kolor tworzonego klocka.
 */
Block::Block(const unsigned int color)
{
	this->color = new Color(color);
}

/**
 * Zainicjalizowanie istniejacym unique_ptr'em
 */
Block::Block(const Color * color)
{
	this->color = new Color(color->getColor());
}

/**
 * Konstruktor kopiujący.
 */
Block::Block(const Block & another)
{
	this->color = new Color(another.getColor()->getColor());
}

/**
 * Konstrutkor w oparciu o przeslany kolor.
 */
Block::Block(const Color & color)
{
	this->color = new Color(color.getColor());
}

/**
 * Dekstrutkor.
 */
Block::~Block()
{
	delete(color);
}

/**
 * Operator przypisania
 */
Block& Block::operator=(const Block & rhs)
{
	if( *this == rhs)
		return *this;

	delete(color);
	this->color = new Color(rhs.getColor()->getColor());
	return *this;
}

/**
 * Operator porownania dwoch klockow. Sa takie same jesli maja takie same kolory.
 */
bool Block::operator==(const Block & rhs) const
{
	return *(color) == *(rhs.color);
}

/*
 * Operator porownania dwoch klockow.
 */
bool Block::operator <(const Block & rhs) const
{
	return *(color) < *(rhs.color);
}

/*
 * Zwraca wskaznik na kolor tego klocka
 */
Color* Block::getColor() const
{
	return color;
}
