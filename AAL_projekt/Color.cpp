#include "Color.h"

/*
 * Konstruktor przyjmujacy jako argument numer koloru.
 */
Color::Color(unsigned int thisColor) : thisColorNo(thisColor) { }

/*
 * Destruktor.
 */
Color::~Color()
{
}

/*
 * Pobiera ilosc kolorow definiowanych dla tego przypadku.
 */
unsigned int Color::getNumberOfAllColors()
{
	return numberOfAllColors;
}

/*
 * Ustawia ilosc wszystkich kolorow.
 */
void Color::setNumberOfAllColors(unsigned int number)
{
	numberOfAllColors = number;
}

/*
 * Operator porownania czy dwa kolory sa identyczne.
 */
bool Color::operator==(const Color & rhs)
{
	return thisColorNo == rhs.getColor();
}