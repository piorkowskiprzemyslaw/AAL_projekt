#ifndef SELECTIONSORT_H
#define SELECTIONSORT_H

#include <iostream>

/*
 * Funkcja stosująca sortowanie przez wstawienie.
 */
int selectionSort(std::pair<unsigned int, unsigned int> * table, unsigned int N)
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

	return 0;
}

#endif // SELECTIONSORT_H
