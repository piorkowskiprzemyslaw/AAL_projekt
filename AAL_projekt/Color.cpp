#include "Color.h"

unsigned int Color::numberOfAllColors;

/*
 * Konstruktor przyjmujacy jako argument numer koloru.
 */
Color::Color(unsigned int thisColor) : thisColorNo(thisColor) {}

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
	return Color::numberOfAllColors;
}

/*
 * Ustawia ilosc wszystkich kolorow.
 */
void Color::setNumberOfAllColors(const unsigned int number)
{
	Color::numberOfAllColors = number;
}

/*
 * Pobiera numer tego koloru.
 */
unsigned int Color::getColor() const
{
	return thisColorNo;
}

/*
 * Operator porownania czy dwa kolory sa identyczne.
 */
bool Color::operator==(const Color & rhs)
{
	return thisColorNo == rhs.getColor();
}

/*
 * Opeator mniejszosci.
 */
bool Color::operator<(const Color & rhs)
{
	return thisColorNo < rhs.thisColorNo;
}
