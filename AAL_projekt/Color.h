#ifndef COLOR_H
#define COLOR_H

/*
 * Klasa reprezentujaca kolor.
 */
class Color
{
private:
	unsigned int thisColorNo;
	static unsigned int numberOfAllColors;
public:
	Color(unsigned int thisColor);
	~Color();
	Color& operator=(const Color & rhs);
	static unsigned int getNumberOfAllColors();
	static void setNumberOfAllColors(const unsigned int number);
	unsigned int getColor() const;
	bool operator==(const Color & rhs);
	bool operator<(const Color & rhs);
};

#endif
