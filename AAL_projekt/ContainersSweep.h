#ifndef CONTAINERSSWEEP_H
#define CONTAINERSSWEEP_H

#include <iostream>
#include <cstring>
#include "ContainerSet.h"
#include "ColorCompare.h"

/*
 * Klasa realizujaca podejscie do rozwiazania problemu za pomoca omiatania pojemnikow z klockow
 * o kolorach ktore juz nie moga sie w tym pojemniku znajdowac.
 */
template <typename T>
class ContainersSweep
{
private:
	ContainerSet * containerSet;
	size_t * colorMultiplicity;
	unsigned int counter;
	bool checkPreconditions();
	void organizeColor(const std::map<Color, bool, ColorCompare> & table, const Color & color);
	T sort;
public:
	ContainersSweep();
	ContainersSweep(const ContainerSet & containerSet);
	~ContainersSweep();
	void showContainerSet( );
	void solveProblem();
	unsigned int getCounter( ) const;
	bool getState() const;
};

/*
* Domyslny konstruktor.
*/
template <typename T >
ContainersSweep<T>::ContainersSweep()
{
	containerSet = nullptr;
	colorMultiplicity = nullptr;
	counter = 0;
}

/*
* Konstrutkor pobierajacy konkretny zbior pojemnikow.
*/
template <typename T>
ContainersSweep<T>::ContainersSweep(const ContainerSet & containerSet)
{
	this->containerSet = new ContainerSet(containerSet);
	colorMultiplicity = new size_t[this->containerSet->getColorsNumber()];
	counter = 0;
}

/*
* Destruktor.
*/
template <typename T>
ContainersSweep<T>::~ContainersSweep()
{
	delete(containerSet);
	delete(colorMultiplicity);
}

/*
* Sprawdz warunki poczatkowe.
*/
template <typename T>
bool ContainersSweep<T>::checkPreconditions()
{
    unsigned int allColorSum = 0;

	//liczba kolorow musi byc mniejsza badz rowna liczbe dostepnych pojemnikow.
	if (Color::getNumberOfAllColors() > this->containerSet->size())
	{
		return false;
	}

	//lacznie we wszystkich pojemnikach jest nie wiecej niz n klockow danego koloru.
	//przy okazji wykonuje zliczanie każdego koloru - potrzebne do dalszej czesci algorytmu,
	for (size_t i = 0; i < containerSet->getColorsNumber(); i++)
	{
		colorMultiplicity[i] = containerSet->colorMultiplicity(i);
		allColorSum += colorMultiplicity[i];
		if (colorMultiplicity[i] > containerSet->size())
		{
			return false;
		}

	}

	//zapewnienie aby wszystkie klocki miescily sie w pojemnikach, oraz zeby dalo
	//sie wykonac ruch.
	if (allColorSum >= containerSet->getCapacitySum())
	{
		return false;
	}

	return true;
}

/*
* Metoda rozwiazujaca problem implementowanym algorytmem.
*/
template <typename T>
void ContainersSweep<T>::solveProblem()
{
	// mapowanie koloru na stan uporzadkowania.
	std::map<Color, bool , ColorCompare> colorOrganizeMap;
    //talibca z parami <kolor, ilosc wystapien> potrzebna do zrealizowania algorytmu.
    std::vector<std::pair<Color, unsigned int> > table(0);

    // Inicjalizowanie mapy, na poczatku zaden kolor nie jest uporzadkowany.
	for ( size_t i = 0; i < Color::getNumberOfAllColors( ); ++i ) {
    	colorOrganizeMap.emplace(i , false);
	}

    // Inicjalizowanie vectora.
	for ( size_t i = 0; i < Color::getNumberOfAllColors( ); ++i ) {
    	table.emplace_back(i , containerSet->colorMultiplicity(i));
	}

	std::cout << "Solving problem with ContainersSweep" << std::endl;
	std::cout << "Checking preconditions ... ";

	//sprawdzanie warunkow poczatkowych umozliwiajacych rozwiazanie zadania.
	if ( !checkPreconditions( ) ) {
		std::cout << "[FALSE]" << std::endl;
		std::cout << "End of solving problem" << std::endl;
		return;
	}
	std::cout << "[OK]" << std::endl;
	sort(table);
	//testowe wypisanie tabeli
	for( size_t i = 0 ; i < containerSet->getColorsNumber() ; ++i)
    {
        std::cout << "Color[" << table[i].first.getColor() << "] : " << table[i].second << std::endl;
    }

	while ( containerSet->checkState( ) == false ) {
		//organizowanie kolorów, od najbardziej licznego do najmniej.
		for( size_t i = 0 ; i < table.size() ; ++i)
		{
			colorOrganizeMap[table[i].first] = true;
			organizeColor(colorOrganizeMap ,table[i].first);
		}
		break;
	}

}

/*
 * Metoda realizujaca sprzatanie konkretnego koloru w pudelkach.
 * Table to vector wartosci logicznych mowiacy czy kolor byl juz rozpatrywany.
 */
template <typename T>
void ContainersSweep<T>::organizeColor(const std::map<Color, bool, ColorCompare> & colorOrganizeMap, const Color & color)
{
	// Itearator na pojemnik z najwieksza liczba klockow koloru color
    ContainerSet::iterator actual = containerSet->getMaxiumWithColor(color);
	// Iterator na prawego sasiada
    ContainerSet::iterator rightNeighbour(actual);
    ++rightNeighbour;
	// Pomocniczy iterator
	ContainerSet::iterator tmpIter( rightNeighbour );
	// Iterator na pojemnik z wolnym miejscem.
	ContainerSet::iterator freeSpace( rightNeighbour );
    // liczba klockow koloru color w actual
	unsigned int blocksNumer = 0;
	// Vector wartosci logicznych mowiacy czy kontener o indeksie rownym indeksowi w tablicy zostal juz uporzadkowany.
	std::vector<bool> containerOrganized(containerSet->size(),false);
	// Pomocniczy pointer na Color
	Color* tmpColorPointer;
	// Pomocnicza flaga logiczna
	bool flag = false;

    // Omiatamy dopoki maksymalna ilosc znaleziona w pojemniku jest wieksza od 1.
    while(actual->getColorMultiplicity(color) > 1)
    {
		blocksNumer = actual->getColorMultiplicity(color);

		std::cout << "Licznosc koloru " << color.getColor() << " w pojemniku o indeksie " << actual->getIndex() << " = " << blocksNumer << std::endl;

    	// Przekladamy wszystkie nadmiarowe kulki do prawych sasiadow.
    	for(size_t i = 1 ; i < blocksNumer ; ++i) {

    		// Przypadki kiedy prawy sasiad ma flage "false"
    		if(false == containerOrganized[rightNeighbour->getIndex()])
    		{
				//Prawy sasiad ma falge flase i ma wolne miejsce
				if(rightNeighbour->getLeftPlace() != 0)
				{
					std::cout << "Prawy sasiad ma falge flase i ma wolne miejsce"<< std::endl;
					actual->moveBlock(color, *rightNeighbour);
					++counter;
					containerSet->showInfo( );
					continue;
				}

				//Prawy sasiad ma flage false i nie ma wolnego miejsca ale u nas jest wolne miejsce
				if(rightNeighbour->getLeftPlace() == 0 && actual->getLeftPlace() != 0)
				{
					std::cout << "Prawy sasiad ma flage false i nie ma wolnego miejsca ale u nas jest wolne miejsce" << std::endl;

					tmpColorPointer = rightNeighbour->checkIsColorPresent(colorOrganizeMap);
					if (tmpColorPointer != nullptr) {
						// Nie rozpatrywany kolor znajduje sie u sąsiada. Wykonanie swapa.
						std::cout << "Prawy sasiad ma kolor jeszcze nie rozpatrywany." << std::endl;
						rightNeighbour->moveBlock(*tmpColorPointer, *actual);
						actual->moveBlock(color, *rightNeighbour);
						counter += 2;
					} else {
						// Nie rozpatrywany kolor nie znajduje sie u sąsiada, szukamy na prawo
						std::cout << "Prawy sasiad nie ma koloru jeszcze nie rozpatrywanego, szukam w prawych sasiadach." << std::endl;

						while ( ++rightNeighbour != actual ) {

							if ( true == containerOrganized[rightNeighbour->getIndex( )] && rightNeighbour->getLeftPlace() > 0) {

								tmpIter = rightNeighbour;

								while ( ++tmpIter != actual ) {
									tmpColorPointer = tmpIter->checkIsColorPresent( colorOrganizeMap );
									if ( false == containerOrganized[tmpIter->getIndex( )] && tmpColorPointer != nullptr ) {
										counter += containerSet->swapBlockFreeSpace( rightNeighbour, tmpIter, *tmpColorPointer );
										counter += containerSet->swapBlockFreeSpace( tmpIter, actual, color );
										flag = true;
										break;
									}
								}

								if ( flag ) {
									flag = false;
									break;
								}

							} else if ( false == containerOrganized[rightNeighbour->getIndex( )] && rightNeighbour->getLeftPlace( ) > 0 ) {
								counter += containerSet->swapBlockFreeSpace( rightNeighbour, actual, color );
								break;
							} else if ( false == containerOrganized[rightNeighbour->getIndex( )] && nullptr != rightNeighbour->checkIsColorPresent( colorOrganizeMap ) ) {
								tmpColorPointer = rightNeighbour->checkIsColorPresent( colorOrganizeMap );
								counter += containerSet->swapBlocksWithFreeSpace( actual, actual, color, rightNeighbour, *tmpColorPointer );
								break;
							}
						}
					}
					containerSet->showInfo( );
					continue;
				}

				//Prawy sasaiad ma flage flase, nie ma wolnego miejsca, w akualnie rozpatrywanym pojemniku tez nie ma wolnego miejsca.
				if(rightNeighbour->getLeftPlace() == 0 && actual->getLeftPlace() == 0)
				{
					std::cout << "Prawy sasaiad ma flage flase, nie ma wolnego miejsca, w akualnie rozpatrywanym pojemniku tez nie ma wolnego miejsca." << std::endl;

					// Znalezienie wolnego miejsca.
					freeSpace = actual;

					while ( ++freeSpace != actual ) {
						if ( freeSpace->getLeftPlace( ) > 0 ) {
							break;
						}
					}

					if ( freeSpace == actual ) {
						std::cerr << "Nie znaleziono wolnego miejsca !" << std::endl;
						return;
					}

					tmpColorPointer = rightNeighbour->checkIsColorPresent( colorOrganizeMap );
					if ( tmpColorPointer != nullptr ) {
						std::cout << "Prawy sasiad ma kolor jeszcze nie rozpatrywany. Wykonanie zamieniany klockow miejscami." << std::endl;
						counter += containerSet->swapBlocksWithFreeSpace( freeSpace, actual, color, rightNeighbour, *tmpColorPointer );
					} else {
						// Nie rozpatrywany kolor nie znajduje sie u sąsiada, szukamy na prawo
						std::cout << "Prawy sasiad nie ma koloru jeszcze nie rozpatrywanego, szukam w prawych sasiadach." << std::endl;

						while ( ++rightNeighbour != actual ) {

							if ( true == containerOrganized[rightNeighbour->getIndex( )] && rightNeighbour->getLeftPlace( ) > 0 ) {

								tmpIter = rightNeighbour;
								while ( ++tmpIter != actual ) {
									tmpColorPointer = tmpIter->checkIsColorPresent( colorOrganizeMap );
									if ( false == containerOrganized[tmpIter->getIndex( )] && tmpColorPointer != nullptr ) {
										counter += containerSet->swapBlockFreeSpace( rightNeighbour, tmpIter, *tmpColorPointer );
										counter += containerSet->swapBlockFreeSpace( tmpIter, actual, color );
										flag = true;
										break;
									}
								}

								if ( flag ) {
									flag = false;
									break;
								}

							} else if ( false == containerOrganized[rightNeighbour->getIndex( )] && rightNeighbour->getLeftPlace( ) > 0 ) {
								counter += containerSet->swapBlockFreeSpace( rightNeighbour, actual, color );
								break;
							} else if ( false == containerOrganized[rightNeighbour->getIndex( )] && nullptr != rightNeighbour->checkIsColorPresent( colorOrganizeMap ) ) {
								tmpColorPointer = rightNeighbour->checkIsColorPresent( colorOrganizeMap );
								counter += containerSet->swapBlocksWithFreeSpace( freeSpace, actual, color, rightNeighbour, *tmpColorPointer );
								break;
							}
						}
					}
					containerSet->showInfo( );
					continue;
				}
    		} else /* (true == containerOrganized[rightNeighbour->getIndex()]) */ {

    			// Prawy sasiad ma flage true i ma wolone miejsce.
    			if(rightNeighbour->getLeftPlace() != 0)
    			{
    				std::cout << "Prawy sasiad ma flage true i ma wolone miejsce." <<std::endl;

					tmpIter = rightNeighbour;

					while ( ++tmpIter != rightNeighbour )  {
						tmpColorPointer = tmpIter->checkIsColorPresent( colorOrganizeMap );
						if ( nullptr != tmpColorPointer && false == containerOrganized[tmpIter->getIndex( )] ) {
							counter += containerSet->swapBlockFreeSpace( rightNeighbour, tmpIter, *tmpColorPointer );
							counter += containerSet->swapBlockFreeSpace( tmpIter, actual, color );
							std::cout << "Znalazlem kandydata do zamiany i wykonalem zamiane." << std::endl;
							break;
						}
					}
					containerSet->showInfo( );
    				continue;
    			}

    			// Prawy sąsiad ma flage true i nie ma wolnego miejsca, ale w aktualnie rozpatrywanym
    			// pojemniku jest wolne miejsce.
    			if(rightNeighbour->getLeftPlace() == 0 && actual->getLeftPlace() != 0)
    			{
    				std::cout << "Prawy sąsiad ma flage true i nie ma wolnego miejsca, ale w aktualnie rozpatrywanym pojemniku jest wolne miejsce." << std::endl;

					while ( ++rightNeighbour != actual ) {

						if ( false == containerOrganized[rightNeighbour->getIndex( )] && rightNeighbour->getLeftPlace( ) != 0 ) {
							counter += containerSet->swapBlockFreeSpace( rightNeighbour, actual, color );
							std::cout << "Znalazlem kandydata do zamiany"<<std::endl;
							break;
						} else if ( false == containerOrganized[rightNeighbour->getIndex( )] && nullptr != rightNeighbour->checkIsColorPresent( colorOrganizeMap ) ) {
							tmpColorPointer = rightNeighbour->checkIsColorPresent( colorOrganizeMap );
							counter += containerSet->swapBlocksWithFreeSpace( actual, actual, color, rightNeighbour, *tmpColorPointer );
							std::cout << "Znalazlem kandydata do zamiany"<<std::endl;
							break;
						} else if ( true == containerOrganized[rightNeighbour->getIndex( )] && rightNeighbour->getLeftPlace( ) != 0 ) {

							tmpIter = rightNeighbour;

							while ( ++tmpIter != actual ) {
								tmpColorPointer = tmpIter->checkIsColorPresent( colorOrganizeMap );
								if ( false == containerOrganized[tmpIter->getIndex( )] && nullptr != tmpColorPointer ) {
									counter += containerSet->swapBlockFreeSpace( rightNeighbour, tmpIter, *tmpColorPointer );
									counter += containerSet->swapBlockFreeSpace( tmpIter, actual, color );
									flag = true;
									std::cout << "Znalazlem kandydata do zamiany"<<std::endl;
									break;
								}
							}

							if ( flag ) {
								flag = false;
								break;
							}
						}
					}
					containerSet->showInfo( );
    				continue;
    			}

    			// Prawy sasiad ma flage true i nie ma wolnego miejsca, w aktualnie rozpatrywanym
    			// pojemniku tez nie ma wolnego miejsca.
    			if(rightNeighbour->getLeftPlace() == 0 && actual->getLeftPlace() == 0)
    			{
    				std::cout << "Prawy sasiad ma flage true i nie ma wolnego miejsca, w aktualnie rozpatrywanym pojemniku tez nie ma wolnego miejsca." << std::endl;

					// Znalezienie wolnego miejsca.
					freeSpace = actual;

					while ( ++freeSpace != actual ) {
						if ( freeSpace->getLeftPlace( ) > 0 ) {
							break;
						}
					}

					if ( freeSpace == actual ) {
						std::cerr << "Nie znaleziono wolnego miejsca !" << std::endl;
						return;
					}

					while ( ++rightNeighbour != actual ) {

						if ( false == containerOrganized[rightNeighbour->getIndex( )] && rightNeighbour->getLeftPlace( ) != 0 ) {
							counter += containerSet->swapBlockFreeSpace( rightNeighbour, actual, color );
							std::cout << "Znalazlem kandydata do zamiany"<<std::endl;
							break;
						} else if ( false == containerOrganized[rightNeighbour->getIndex( )] && nullptr != rightNeighbour->checkIsColorPresent( colorOrganizeMap ) ) {
							tmpColorPointer = rightNeighbour->checkIsColorPresent( colorOrganizeMap );
							counter += containerSet->swapBlocksWithFreeSpace( freeSpace, actual, color, rightNeighbour, *tmpColorPointer );
							std::cout << "Znalazlem kandydata do zamiany"<<std::endl;
							break;
						} else if ( true == containerOrganized[rightNeighbour->getIndex( )] && rightNeighbour->getLeftPlace( ) != 0 ) {

							tmpIter = rightNeighbour;

							while ( ++tmpIter != actual ) {
								tmpColorPointer = tmpIter->checkIsColorPresent( colorOrganizeMap );
								if ( false == containerOrganized[tmpIter->getIndex( )] && nullptr != tmpColorPointer ) {
									counter += containerSet->swapBlockFreeSpace( rightNeighbour, tmpIter, *tmpColorPointer );
									counter += containerSet->swapBlockFreeSpace( tmpIter, actual, color );
									std::cout << "Znalazlem kandydata do zamiany"<<std::endl;
									flag = true;
									break;
								}
							}

							if ( flag ) {
								flag = false;
								break;
							}
						}
					}
					containerSet->showInfo( );
    				continue;
    			}

    		}
    	}

		std::cout << std::endl << "Kolor numer "<< color.getColor() << " w pojemniku o indeksie "<< actual->getIndex() << " uporzadkowany..." << std::endl;
		containerSet->showInfo( );

		containerOrganized[actual->getIndex( )] = true;
    	actual = containerSet->getMaxiumWithColor(color);
    	rightNeighbour = actual;
    	rightNeighbour++;
    }
}

/**
 * Pokazuje zawartosc konterow oraz ich stany.
 */
template<typename T>
void ContainersSweep<T>::showContainerSet( )
{
	containerSet->showInfo( );
}

/**
 * Pobranie licznika ruchow.
 */
template <typename T>
unsigned int ContainersSweep<T>::getCounter( ) const
{
	return counter;
}

/**
 * Sprawdzenie stanu w jakim znajduja sie pojemniki.
 */
template <typename T>
bool ContainersSweep<T>::getState() const
{
	return containerSet->checkState();
}

#endif
