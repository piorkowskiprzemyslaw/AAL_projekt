#include "Color.h"

unsigned int Color::numberOfAllColors;

/**
 * Konstruktor domyslny, tworzy kolor o numerze zero
 */
Color::Color() : thisColorNo(0) {}


/*
 * Konstruktor przyjmujacy jako argument numer koloru.
 */
Color::Color(const unsigned int thisColor) : thisColorNo(thisColor) {}

/*
 * Destruktor.
 */
Color::~Color() {}

/**
 * Operator przypisania.
 */
Color& Color::operator=(const Color& rhs)
{
	if(*this == rhs)
		return *this;
	thisColorNo = rhs.thisColorNo;
	return *this;
}

/**
 * Operator wyluskania
 */
const Color* Color::operator *() const
{
	return this;
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
bool Color::operator==(const Color & rhs) const
{
	return thisColorNo == rhs.getColor();
}

/*
 * Opeator mniejszosci.
 */
bool Color::operator<(const Color & rhs) const
{
	return thisColorNo < rhs.thisColorNo;
}
