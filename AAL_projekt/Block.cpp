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

/**
 * Konstruktor kopiujący.
 */
Block::Block(const Block & another)
{
	unsigned int anotherColor = another.getColor()->getColor();
	this->color = std::unique_ptr<Color>(new Color(anotherColor));
}

/**
 * Konstrutkor w oparciu o przeslany kolor.
 */
Block::Block(const Color & color)
{
	this->color = std::unique_ptr<Color>(new Color(color.getColor()));
}

/*
 * Dekstrutkor.
 */
Block::~Block() { }

/**
 * Operator przypisania
 */
Block& Block::operator=(const Block & rhs)
{
	if( *this == rhs)
		return *this;

	unsigned int rhsColor = rhs.getColor()->getColor();
	this->color = std::unique_ptr<Color>(new Color(rhsColor));

	return *this;
}

/*
 * Operator porownania dwoch klockow. Sa takie same jesli maja takie same kolory.
 */
bool Block::operator==(const Block & rhs) const
{
	return *(color.get()) == *(rhs.color.get());
}

/*
 * Operator porownania dwoch klockow.
 */
bool Block::operator <(const Block & rhs) const
{
	return *(color.get()) < *(rhs.color.get());
}

/*
 * Zwraca wskaznik na kolor tego klocka
 */
Color* Block::getColor() const
{
	return this->color.get();
}

