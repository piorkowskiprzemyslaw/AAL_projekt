#ifndef SELECTIONSORT_H
#define SELECTIONSORT_H

#include <iostream>

/**
 * Klasa reprezentujaca selection sort;
 */
class SelectionSort{
private:

public:
	void operator()(std::pair<unsigned int, unsigned int> * table, unsigned int N)
	{
	    std::pair<unsigned int, unsigned int> tmp;
	    unsigned int minimum;

		for(unsigned int i = 0 ; i < N ; ++i)
		{
	        minimum = i;

	        for(unsigned int j = i + 1; j < N ; ++j)
	        {
	            if(table[j].second > table[minimum].second)
	            {
	                minimum = j;
	            }
	        }

	        tmp = table[i];
	        table[i] = table[minimum];
	        table[minimum] = tmp;
		}
	}

	void operator()(std::vector<std::pair<Color, unsigned int> > & vector)
	{
		std::pair<Color, unsigned int> tmp;
		unsigned int minimum;

		for(size_t i = 0 ; i < vector.size() ; ++i)
		{
			minimum = i;

			for(unsigned int j = i + 1 ; j < vector.size() ; ++j)
			{
				if(vector[j].second > vector[minimum].second)
				{
					minimum = j;
				}
			}

			tmp = vector[i];
			vector[i] = vector[minimum];
			vector[minimum] = tmp;
		}
	}
};

#endif // SELECTIONSORT_H
