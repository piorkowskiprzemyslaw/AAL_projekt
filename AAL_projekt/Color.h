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
	Color();
	Color(const unsigned int thisColor);
	~Color();
	Color& operator=(const Color & rhs);
	//const Color& operator*() const;
	unsigned int getColor() const;
	bool operator==(const Color & rhs) const;
	bool operator<(const Color & rhs) const;

	static unsigned int getNumberOfAllColors();
	static void setNumberOfAllColors(const unsigned int number);
};

#endif
