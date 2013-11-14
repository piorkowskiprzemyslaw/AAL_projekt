#include "Container.h"

/*
 * Konstruktor przyjmuj¹cy wektor z zawieranymi kolorami w tym pojemniku.
 */
Container::Container(std::vector<unsigned int> & included)
{
	for (size_t i = 0; i < included.size(); i++)
	{
		includedBlocks.push_back(new Block(included[i]));
	}
}

/*
 * Destruktor.
 */
Container::~Container(){ }

