#ifndef CONTAINERSSWEEP_H
#define CONTAINERSSWEEP_H

#include <iostream>
#include <cstring>
#include "ContainerSet.h"
#include "ColorCompare.h"

/**
 * Enum okreslajacy sposob wykonania omiatania.
 */
enum Mode
{
	oneDir,
	twoDir
};

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
	bool checkPreconditions( );
	void organizeColorOneDir( const std::map<Color, bool, ColorCompare> & table, const Color & color );
	void organizeColorTwoDir( const std::map<Color, bool, ColorCompare> & table, const Color & color );
	T sort;
public:
	ContainersSweep( );
	ContainersSweep( const ContainerSet & containerSet );
	~ContainersSweep( );
	void showContainerSet( );
	void solveProblem( const Mode mode );
	unsigned int getCounter( ) const;
	bool getState( ) const;
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
void ContainersSweep<T>::solveProblem(const Mode mode)
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

	//organizowanie kolorów, od najbardziej licznego do najmniej.
	for( size_t i = 0 ; i < table.size() ; ++i)
	{
		colorOrganizeMap[table[i].first] = true;
		if ( mode == oneDir ) {
			organizeColorOneDir( colorOrganizeMap, table[i].first );
		} else {
			organizeColorTwoDir( colorOrganizeMap, table[i].first );
		}
		if ( COLORVERBOSE ) {
			std::cout << "Kolor numer " << table[i].first.getColor() << " uporzadkowany. Pozostalo jeszcze " << table.size( ) - i - 1 << " kolorow." << std::endl;
		}
	}

}

/**
 * Metoda realizujaca sprzatanie konkretnego koloru w pudelkach omiatajac tylko w jedną strone.
 * Table to vector wartosci logicznych mowiacy czy kolor byl juz rozpatrywany.
 */
template <typename T>
void ContainersSweep<T>::organizeColorOneDir(const std::map<Color, bool, ColorCompare> & colorOrganizeMap, const Color & color)
{
	// Itearator na pojemnik z najwieksza liczba klockow koloru color
    ContainerSet::iterator actual = containerSet->getMaxiumWithColor(color);
	// Iterator na prawego sasiada
    ContainerSet::iterator rightNeighbour(actual);
    ++rightNeighbour;
	// Tymczasowy iterator na prawego sasiada - slozy do przeszukiwania wsrod prawych sasiadow.
	ContainerSet::iterator tmpRightNeighbour( rightNeighbour );
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

		if ( VVERBOSE ) {
			std::cout << "Licznosc koloru " << color.getColor() << " w pojemniku o indeksie " << actual->getIndex() << " = " << blocksNumer << std::endl;
		}

    	// Przekladamy wszystkie nadmiarowe kulki do prawych sasiadow.
    	for(size_t i = 1 ; i < blocksNumer ; ++i) {

    		// Przypadki kiedy prawy sasiad ma flage "false"
    		if(false == containerOrganized[rightNeighbour->getIndex()])
    		{
				//Prawy sasiad ma falge flase i ma wolne miejsce
				if(rightNeighbour->getLeftPlace() != 0)
				{
					actual->moveBlock(color, *rightNeighbour);
					++counter;
					if ( VVERBOSE ) {
						std::cout << "Prawy sasiad ma falge flase i ma wolne miejsce"<< std::endl;
						containerSet->showInfo( );
					}
					continue;
				}

				//Prawy sasiad ma flage false i nie ma wolnego miejsca ale u nas jest wolne miejsce
				if(rightNeighbour->getLeftPlace() == 0 && actual->getLeftPlace() != 0)
				{
					if ( VVERBOSE ) {
						std::cout << "Prawy sasiad ma flage false i nie ma wolnego miejsca ale u nas jest wolne miejsce" << std::endl;
					}

					tmpColorPointer = rightNeighbour->checkIsColorPresent(colorOrganizeMap);
					if (tmpColorPointer != nullptr) {

						// Nie rozpatrywany kolor znajduje sie u sąsiada. Wykonanie swapa.
						if ( VVERBOSE ) {
							std::cout << "Prawy sasiad ma kolor jeszcze nie rozpatrywany." << std::endl;
						}
						rightNeighbour->moveBlock(*tmpColorPointer, *actual);
						actual->moveBlock(color, *rightNeighbour);
						counter += 2;

					} else {

						// Nie rozpatrywany kolor nie znajduje sie u sąsiada, szukamy na prawo
						if ( VVERBOSE ) {
							std::cout << "Prawy sasiad nie ma koloru jeszcze nie rozpatrywanego, szukam w prawych sasiadach." << std::endl;
						}
						flag = false;
						tmpRightNeighbour = rightNeighbour;

						while ( ++tmpRightNeighbour != actual ) {

							if ( true == containerOrganized[tmpRightNeighbour->getIndex( )] && tmpRightNeighbour->getLeftPlace() != 0) {

								tmpIter = tmpRightNeighbour;

								while ( ++tmpIter != actual ) {

									tmpColorPointer = tmpIter->checkIsColorPresent( colorOrganizeMap );
									if ( false == containerOrganized[tmpIter->getIndex( )] && tmpColorPointer != nullptr ) {
										counter += containerSet->swapBlockFreeSpace( tmpRightNeighbour, tmpIter, *tmpColorPointer );
										counter += containerSet->swapBlockFreeSpace( tmpIter, actual, color );
										flag = true;
										break;
									}
								}

								if ( flag ) {
									break;
								}

							} else if ( false == containerOrganized[tmpRightNeighbour->getIndex( )] && tmpRightNeighbour->getLeftPlace( ) != 0 ) {

								counter += containerSet->swapBlockFreeSpace( tmpRightNeighbour, actual, color );
								flag = true;
								break;

							} else if ( false == containerOrganized[tmpRightNeighbour->getIndex( )] && nullptr != tmpRightNeighbour->checkIsColorPresent( colorOrganizeMap ) ) {

								tmpColorPointer = tmpRightNeighbour->checkIsColorPresent( colorOrganizeMap );
								counter += containerSet->swapBlocksWithFreeSpace( actual, actual, color, tmpRightNeighbour, *tmpColorPointer );
								flag = true;
								break;

							}
						}

						if ( flag == false ) {
							std::cerr << "Nie ma mozliwosci rozlozenia [0]" << std::endl;
							exit( 1 );
						}

						flag = false;
					}


					if ( VVERBOSE ) {
						containerSet->showInfo( );
					}
					continue;
				}

				//Prawy sasaiad ma flage flase, nie ma wolnego miejsca, w akualnie rozpatrywanym pojemniku tez nie ma wolnego miejsca.
				if(rightNeighbour->getLeftPlace() == 0 && actual->getLeftPlace() == 0) {

					if ( VVERBOSE ) {
						std::cout << "Prawy sasaiad ma flage flase, nie ma wolnego miejsca, w akualnie rozpatrywanym pojemniku tez nie ma wolnego miejsca." << std::endl;
					}
					// Znalezienie wolnego miejsca.
					freeSpace = actual;
					while ( ++freeSpace != actual ) {
						if ( freeSpace->getLeftPlace( ) > 0 ) {
							break;
						}
					}

					tmpColorPointer = rightNeighbour->checkIsColorPresent( colorOrganizeMap );
					if ( tmpColorPointer != nullptr ) {
					
						if ( VVERBOSE ) {
							std::cout << "Prawy sasiad ma kolor jeszcze nie rozpatrywany. Wykonanie zamiany klockow miejscami." << std::endl;
						}
						counter += containerSet->swapBlocksWithFreeSpace( freeSpace, actual, color, rightNeighbour, *tmpColorPointer );
					
					} else {
						
						// Nie rozpatrywany kolor nie znajduje sie u sąsiada, szukamy na prawo
						if ( VVERBOSE ) {
							std::cout << "Prawy sasiad nie ma koloru jeszcze nie rozpatrywanego, szukam w prawych sasiadach." << std::endl;
						}
						flag = false;
						tmpRightNeighbour = rightNeighbour;
						while ( ++tmpRightNeighbour != actual ) {

							if ( true == containerOrganized[tmpRightNeighbour->getIndex( )] && tmpRightNeighbour->getLeftPlace( ) > 0 ) {

								tmpIter = tmpRightNeighbour;
								while ( ++tmpIter != actual ) {
									tmpColorPointer = tmpIter->checkIsColorPresent( colorOrganizeMap );
									if ( false == containerOrganized[tmpIter->getIndex( )] && tmpColorPointer != nullptr ) {
										counter += containerSet->swapBlockFreeSpace( tmpRightNeighbour, tmpIter, *tmpColorPointer );
										counter += containerSet->swapBlockFreeSpace( tmpIter, actual, color );
										flag = true;
										break;
									}
								}

								if ( flag ) {
									break;
								}

							} else if ( false == containerOrganized[tmpRightNeighbour->getIndex( )] && tmpRightNeighbour->getLeftPlace( ) > 0 ) {
								counter += containerSet->swapBlockFreeSpace( tmpRightNeighbour, actual, color );
								flag = true;
								break;
							} else if ( false == containerOrganized[tmpRightNeighbour->getIndex( )] && nullptr != tmpRightNeighbour->checkIsColorPresent( colorOrganizeMap ) ) {
								tmpColorPointer = tmpRightNeighbour->checkIsColorPresent( colorOrganizeMap );
								counter += containerSet->swapBlocksWithFreeSpace( freeSpace, actual, color, tmpRightNeighbour, *tmpColorPointer );
								flag = true;
								break;
							}
						}

						if ( flag == false ) {
							std::cerr << "Nie ma mozliwosci rozlozenia [1]" << std::endl;
							exit( 1 );
						}

					}


					flag = false;

					if ( VVERBOSE ) {
						containerSet->showInfo( );
					}
					continue;
				}
    		} else /* (true == containerOrganized[rightNeighbour->getIndex()]) */ {

    			// Prawy sasiad ma flage true i ma wolone miejsce.
    			if(rightNeighbour->getLeftPlace() != 0)
    			{
					flag = false;
					if ( VVERBOSE ) {
    					std::cout << "Prawy sasiad ma flage true i ma wolone miejsce." <<std::endl;
					}

					tmpIter = rightNeighbour;

					while ( ++tmpIter != rightNeighbour )  {
						tmpColorPointer = tmpIter->checkIsColorPresent( colorOrganizeMap );
						if ( nullptr != tmpColorPointer && false == containerOrganized[tmpIter->getIndex( )] ) {
							counter += containerSet->swapBlockFreeSpace( rightNeighbour, tmpIter, *tmpColorPointer );
							counter += containerSet->swapBlockFreeSpace( tmpIter, actual, color );
							flag = true;
							if ( VVERBOSE ) {
								std::cout << "Znalazlem kandydata do zamiany i wykonalem zamiane." << std::endl;
							}
							break;
						}
					}

					// Jesli nie zostal znaleziony zaden kandydat do zamiany, to szukamy wolnego miejsca w 
					// pojemnikach po prawej stronie. Miejsce takie musi sie znalezc.
					if ( !flag ) {
						tmpIter = rightNeighbour;
						while ( ++tmpIter != rightNeighbour ) {
							if ( false == containerOrganized[tmpIter->getIndex( )] && tmpIter->getLeftPlace( ) != 0 ) {
								counter += containerSet->swapBlockFreeSpace( tmpIter, actual, color );
								flag = true;
								if ( VVERBOSE ) {
									std::cout << "Znalazlem kandydata z flaga false i wolnym miejscem, przenosze tam klocek." << std::endl;
								}
								break;
							}
						}
					}

					if ( flag == false ) {
						std::cerr << "Nie ma mozliwosci rozlozenia [2]" << std::endl;
						exit( 1 );
					}

					flag = false;

					if ( VVERBOSE ) {
						containerSet->showInfo( );
					}

    				continue;
    			}

    			// Prawy sąsiad ma flage true i nie ma wolnego miejsca, ale w aktualnie rozpatrywanym
    			// pojemniku jest wolne miejsce.
    			if(rightNeighbour->getLeftPlace() == 0 && actual->getLeftPlace() != 0) {
					
					flag = false;
					if ( VVERBOSE ) {
    					std::cout << "Prawy sąsiad ma flage true i nie ma wolnego miejsca, ale w aktualnie rozpatrywanym pojemniku jest wolne miejsce." << std::endl;
					}

					tmpRightNeighbour = rightNeighbour;
					while ( ++tmpRightNeighbour != actual ) {

						if ( false == containerOrganized[tmpRightNeighbour->getIndex( )] && tmpRightNeighbour->getLeftPlace( ) != 0 ) {

							counter += containerSet->swapBlockFreeSpace( tmpRightNeighbour, actual, color );
							if ( VVERBOSE ) {
								std::cout << "Znalazlem kandydata do zamiany"<<std::endl;
							}
							flag = true;
							break;

						} else if ( false == containerOrganized[tmpRightNeighbour->getIndex( )] && nullptr != tmpRightNeighbour->checkIsColorPresent( colorOrganizeMap ) ) {

							tmpColorPointer = tmpRightNeighbour->checkIsColorPresent( colorOrganizeMap );
							counter += containerSet->swapBlocksWithFreeSpace( actual, actual, color, tmpRightNeighbour, *tmpColorPointer );
							if ( VVERBOSE ) {
								std::cout << "Znalazlem kandydata do zamiany"<<std::endl;
							}
							flag = true;
							break;

						} else if ( true == containerOrganized[tmpRightNeighbour->getIndex( )] && tmpRightNeighbour->getLeftPlace( ) != 0 ) {

							tmpIter = tmpRightNeighbour;

							while ( ++tmpIter != actual ) {

								tmpColorPointer = tmpIter->checkIsColorPresent( colorOrganizeMap );
								if ( false == containerOrganized[tmpIter->getIndex( )] && nullptr != tmpColorPointer ) {
									counter += containerSet->swapBlockFreeSpace( tmpRightNeighbour, tmpIter, *tmpColorPointer );
									counter += containerSet->swapBlockFreeSpace( tmpIter, actual, color );
									flag = true;
									if ( VVERBOSE ) {
										std::cout << "Znalazlem kandydata do zamiany"<<std::endl;
									}
									break;
								}

							}

							if ( flag ) {
								break;
							}

						}

					}

					if ( flag == false ) {
						std::cerr << "Nie ma mozliwosci rozlozenia [3]" << std::endl;
						exit( 1 );
					}

					flag = false;

					if ( VVERBOSE ) {
						containerSet->showInfo( );
					}
    				continue;
    			}

    			// Prawy sasiad ma flage true i nie ma wolnego miejsca, w aktualnie rozpatrywanym pojemniku tez nie ma wolnego miejsca.
    			if(rightNeighbour->getLeftPlace() == 0 && actual->getLeftPlace() == 0) {
					flag = false;

					if ( VVERBOSE ) {
    					std::cout << "Prawy sasiad ma flage true i nie ma wolnego miejsca, w aktualnie rozpatrywanym pojemniku tez nie ma wolnego miejsca." << std::endl;
					}

					// Znalezienie wolnego miejsca.
					freeSpace = actual;
					while ( ++freeSpace != actual ) {
						if ( freeSpace->getLeftPlace( ) > 0 ) {
							break;
						}
					}

					tmpRightNeighbour = rightNeighbour;
					while ( ++tmpRightNeighbour != actual ) {

						if ( false == containerOrganized[tmpRightNeighbour->getIndex( )] && tmpRightNeighbour->getLeftPlace( ) != 0 ) {

							counter += containerSet->swapBlockFreeSpace( tmpRightNeighbour, actual, color );
							if ( VVERBOSE ) {
								std::cout << "Znalazlem kandydata do zamiany"<<std::endl;
							}
							flag = true;
							break;

						} else if ( false == containerOrganized[tmpRightNeighbour->getIndex( )] && nullptr != tmpRightNeighbour->checkIsColorPresent( colorOrganizeMap ) ) {
							
							tmpColorPointer = tmpRightNeighbour->checkIsColorPresent( colorOrganizeMap );
							counter += containerSet->swapBlocksWithFreeSpace( freeSpace, actual, color, tmpRightNeighbour, *tmpColorPointer );
							if ( VVERBOSE ) {
								std::cout << "Znalazlem kandydata do zamiany"<<std::endl;
							}
							flag = true;
							break;

						} else if ( true == containerOrganized[tmpRightNeighbour->getIndex( )] && tmpRightNeighbour->getLeftPlace( ) != 0 ) {

							tmpIter = tmpRightNeighbour;

							while ( ++tmpIter != actual ) {
								tmpColorPointer = tmpIter->checkIsColorPresent( colorOrganizeMap );
								if ( false == containerOrganized[tmpIter->getIndex( )] && nullptr != tmpColorPointer ) {
									counter += containerSet->swapBlockFreeSpace( tmpRightNeighbour, tmpIter, *tmpColorPointer );
									counter += containerSet->swapBlockFreeSpace( tmpIter, actual, color );
									if ( VVERBOSE ) {
										std::cout << "Znalazlem kandydata do zamiany"<<std::endl;
									}
									flag = true;
									break;
								}
							}

							if ( flag ) {
								break;
							}
						}
					}

					if ( flag == false ) {
						std::cerr << "Nie ma mozliwosci rozlozenia [4]" << std::endl;
						exit( 1 );
					}

					flag = false;

					if ( VVERBOSE ) {
						containerSet->showInfo( );
					}
    				continue;
    			}

    		}
    	}

		if ( CONTAINERVERBOSE ) {
			std::cout << std::endl << "Kolor numer "<< color.getColor() << " w pojemniku o indeksie "<< actual->getIndex() << " uporzadkowany..." << std::endl;
			containerSet->showInfo( );
		}

		containerOrganized[actual->getIndex( )] = true;
    	actual = containerSet->getMaxiumWithColor(color);
    	rightNeighbour = actual;
    	rightNeighbour++;
    }
}

/**
 * Metoda realizujaca sprzatanie koloru color w pudelkach stosujac omiatanie w dwie strony.
 */
template<typename T>
void ContainersSweep<T>::organizeColorTwoDir( const std::map<Color, bool, ColorCompare> & colorOrganizeMap, const Color & color )
{
	// Itearator na pojemnik z najwieksza liczba klockow koloru color.
	ContainerSet::iterator actual = containerSet->getMaxiumWithColor( color );
	// Iterator na sasiada.
	ContainerSet::iterator neighbour( actual );
	// Pomocniczy iterator.
	ContainerSet::iterator tmpIter( neighbour );
	// Iterator na pojemnik z wolnym miejscem.
	ContainerSet::iterator freeSpace( neighbour );
	// Liczba klockow koloru color w actual.
	unsigned int blocksNumer = 0;
	// Vector wartosci logicznych mowiacy czy kontener o indeksie rownym indeksowi w tablicy zostal juz uporzadkowany.
	std::vector<bool> containerOrganized( containerSet->size( ), false );
	// Pomocniczy pointer na Color.
	Color* tmpColorPointer;
	// Pomocnicza flaga logiczna.
	bool flag = false;
	// Zmienna okreslajaca kierunek.
	int dir = 0;

	// Omiatamy dopoki maksymalna ilosc znaleziona w pojemniku jest wieksza od 1.
	while ( actual->getColorMultiplicity( color ) > 1 ) {

		blocksNumer = actual->getColorMultiplicity( color );
		if ( VVERBOSE ) {
			std::cout << "Licznosc koloru " << color.getColor( ) << " w pojemniku o indeksie " << actual->getIndex( ) << " = " << blocksNumer << std::endl;
		}
		
		// Przekladamy wszystkie nadmiarowe kulki do sasiadow.
		for ( size_t i = 1; i < blocksNumer; ++i ) {
			
			neighbour = actual;

			if ( dir & 0x01 ) {
				++neighbour;
			} else {
				--neighbour;
			}

			// Przypadki kiedy sasiad ma flage "false".
			if ( false == containerOrganized[neighbour->getIndex( )] ) {
				
				// Sasiad ma falge flase i ma wolne miejsce.
				if ( neighbour->getLeftPlace( ) != 0 ) {
					actual->moveBlock( color, *neighbour );
					++counter;
					if ( VVERBOSE ) {
						std::cout << "Sasiad ma falge flase i ma wolne miejsce." << std::endl;
						containerSet->showInfo( );
					}
					continue;
				}

				// Sasiad ma flage false i nie ma wolnego miejsca ale w actual jest wolne miejsce.
				if ( neighbour->getLeftPlace( ) == 0 && actual->getLeftPlace( ) != 0 ) {
					if ( VVERBOSE ) {
						std::cout << "Sasiad ma flage false i nie ma wolnego miejsca ale w actual jest wolne miejsce" << std::endl;
					}

					tmpColorPointer = neighbour->checkIsColorPresent( colorOrganizeMap );
					if ( tmpColorPointer != nullptr ) {
						// Nie rozpatrywany kolor znajduje sie u sąsiada. Wykonanie swapa.
						if ( VVERBOSE ) {
							std::cout << "Prawy sasiad ma kolor jeszcze nie rozpatrywany." << std::endl;
						}

						neighbour->moveBlock( *tmpColorPointer, *actual );
						actual->moveBlock( color, *neighbour );
						counter += 2;
					} else {

						// Nie rozpatrywany kolor nie znajduje sie u sąsiada, szukamy na prawo
						if ( VVERBOSE ) {
							std::cout << "Prawy sasiad nie ma koloru jeszcze nie rozpatrywanego, szukam w prawych sasiadach." << std::endl;
						}

						flag = false;

						if ( dir & 0x01 ) {
							++neighbour;
						} else {
							--neighbour;
						}

						while ( neighbour != actual ) {

							if ( true == containerOrganized[neighbour->getIndex( )] && neighbour->getLeftPlace( ) != 0 ) {

								tmpIter = neighbour;

								if ( dir & 0x01 ) {
									++tmpIter;
								} else {
									--tmpIter;
								}

								while ( tmpIter != actual ) {
									tmpColorPointer = tmpIter->checkIsColorPresent( colorOrganizeMap );
									if ( false == containerOrganized[tmpIter->getIndex( )] && tmpColorPointer != nullptr ) {
										counter += containerSet->swapBlockFreeSpace( neighbour, tmpIter, *tmpColorPointer );
										counter += containerSet->swapBlockFreeSpace( tmpIter, actual, color );
										flag = true;
										break;
									}

									if ( dir & 0x01 ) {
										++tmpIter;
									} else {
										--tmpIter;
									}

								}

								if ( flag ) {
									break;
								}

							} else if ( false == containerOrganized[neighbour->getIndex( )] && neighbour->getLeftPlace( ) != 0 ) {

								counter += containerSet->swapBlockFreeSpace( neighbour, actual, color );
								flag = true;
								break;

							} else if ( false == containerOrganized[neighbour->getIndex( )] && nullptr != neighbour->checkIsColorPresent( colorOrganizeMap ) ) {

								tmpColorPointer = neighbour->checkIsColorPresent( colorOrganizeMap );
								counter += containerSet->swapBlocksWithFreeSpace( actual, actual, color, neighbour, *tmpColorPointer );
								flag = true;
								break;

							}

							if ( dir & 0x01 ) {
								++neighbour;
							} else {
								--neighbour;
							}
							
						}

						if ( flag == false ) {
							std::cerr << "Nie ma mozliwosci rozlozenia [0]" << std::endl;
							exit( 1 );
						}

						flag = false;
					}


					if ( VVERBOSE ) {
						containerSet->showInfo( );
					}
					continue;
				}

				// Sasaiad ma flage flase, nie ma wolnego miejsca, w actual tez nie ma wolnego miejsca.
				if ( neighbour->getLeftPlace( ) == 0 && actual->getLeftPlace( ) == 0 ) {

					if ( VVERBOSE ) {
						std::cout << "Sasaiad ma flage flase, nie ma wolnego miejsca, w actual tez nie ma wolnego miejsca." << std::endl;
					}

					// Znalezienie wolnego miejsca.
					freeSpace = actual;

					if ( dir & 0x01 ) {
						++freeSpace;
					} else {
						--freeSpace;
					}

					while ( freeSpace != actual ) {
						if ( freeSpace->getLeftPlace( ) > 0 ) {
							break;
						}
					}

					if ( freeSpace == actual ) {
						std::cerr << "Nie znaleziono wolnego miejsca !" << std::endl;
						return;
					}

					tmpColorPointer = neighbour->checkIsColorPresent( colorOrganizeMap );
					if ( tmpColorPointer != nullptr ) {
						if ( VVERBOSE ) {
							std::cout << "Sasiad ma kolor jeszcze nie rozpatrywany. Wykonanie zamieniany klockow miejscami." << std::endl;
						}

						counter += containerSet->swapBlocksWithFreeSpace( freeSpace, actual, color, neighbour, *tmpColorPointer );
					} else {
						// Nie rozpatrywany kolor nie znajduje sie u sąsiada, szukamy dalej.
						if ( VVERBOSE ) {
							std::cout << "Sasiad nie ma koloru jeszcze nie rozpatrywanego, szukam dalej w tym samym kierunku." << std::endl;
						}

						flag = false;

						if ( dir & 0x01 ) {
							++neighbour;
						} else {
							--neighbour;
						}

						while ( neighbour != actual ) {

							if ( true == containerOrganized[neighbour->getIndex( )] && neighbour->getLeftPlace( ) > 0 ) {

								tmpIter = neighbour;

								if ( dir & 0x01 ) {
									++tmpIter;
								} else {
									--tmpIter;
								}

								while ( tmpIter != actual ) {
									tmpColorPointer = tmpIter->checkIsColorPresent( colorOrganizeMap );
									if ( false == containerOrganized[tmpIter->getIndex( )] && tmpColorPointer != nullptr ) {

										counter += containerSet->swapBlockFreeSpace( neighbour, tmpIter, *tmpColorPointer );
										counter += containerSet->swapBlockFreeSpace( tmpIter, actual, color );
										flag = true;
										break;
									}

									if ( dir & 0x01 ) {
										++tmpIter;
									} else {
										--tmpIter;
									}
								}

								if ( flag ) {
									break;
								}

							} else if ( false == containerOrganized[neighbour->getIndex( )] && neighbour->getLeftPlace( ) > 0 ) {
								counter += containerSet->swapBlockFreeSpace( neighbour, actual, color );
								flag = true;
								break;
							} else if ( false == containerOrganized[neighbour->getIndex( )] && nullptr != neighbour->checkIsColorPresent( colorOrganizeMap ) ) {
								tmpColorPointer = neighbour->checkIsColorPresent( colorOrganizeMap );
								counter += containerSet->swapBlocksWithFreeSpace( freeSpace, actual, color, neighbour, *tmpColorPointer );
								flag = true;
								break;
							}

							if ( dir & 0x01 ) {
								++neighbour;
							} else {
								--neighbour;
							}
						}

						if ( flag == false ) {
							std::cerr << "Nie ma mozliwosci rozlozenia [1]" << std::endl;
							exit( 1 );
						}
					}


					flag = false;

					if ( VVERBOSE ) {
						containerSet->showInfo( );
					}
					
					continue;
				}
			} else /* (true == containerOrganized[neighbour->getIndex()]) */ {

				// Sasiad ma flage true i ma wolone miejsce.
				if ( neighbour->getLeftPlace( ) != 0 ) {
					flag = false;
					if ( VVERBOSE ) {
						std::cout << "Sasiad ma flage true i ma wolone miejsce." << std::endl;
					}

					tmpIter = neighbour;

					if ( dir & 0x01 ) {
						++tmpIter;
					} else {
						--tmpIter;
					}

					while ( tmpIter != neighbour ) {
						tmpColorPointer = tmpIter->checkIsColorPresent( colorOrganizeMap );
						if ( nullptr != tmpColorPointer && false == containerOrganized[tmpIter->getIndex( )] ) {
							counter += containerSet->swapBlockFreeSpace( neighbour, tmpIter, *tmpColorPointer );
							counter += containerSet->swapBlockFreeSpace( tmpIter, actual, color );
							flag = true;
							if ( VVERBOSE ) {
								std::cout << "Znalazlem kandydata do zamiany i wykonalem zamiane." << std::endl;
							}
							break;
						}

						if ( dir & 0x01 ) {
							++tmpIter;
						} else {
							--tmpIter;
						}
					}

					// Jesli nie zostal znaleziony zaden kandydat do zamiany, to szukamy wolnego miejsca w 
					// sasiednich pojemnikach. Miejsce takie musi sie znalezc o ile istnieje wogole mozliwosc
					// rozlozenia klockow.
					if ( !flag ) {
						tmpIter = neighbour;

						if ( dir & 0x01 ) {
							++tmpIter;
						} else {
							--tmpIter;
						}

						while ( tmpIter != neighbour ) {
							if ( false == containerOrganized[tmpIter->getIndex( )] && tmpIter->getLeftPlace( ) != 0 ) {
								counter += containerSet->swapBlockFreeSpace( tmpIter, actual, color );
								flag = true;
								if ( VVERBOSE ) {
									std::cout << "Znalazlem kandydata z flaga false i wolnym miejscem, przenosze tam klocek." << std::endl;
								}
								break;
							}

							if ( dir & 0x01 ) {
								++tmpIter;
							} else {
								--tmpIter;
							}
						}
					}

					if ( flag == false ) {
						std::cerr << "Nie ma mozliwosci rozlozenia [2]" << std::endl;
						exit( 1 );
					}

					flag = false;

					if ( VVERBOSE ) {
						containerSet->showInfo( );
					}

					continue;
				}

				// Sąsiad ma flage true i nie ma wolnego miejsca, ale w actual jest wolne miejsce.
				if ( neighbour->getLeftPlace( ) == 0 && actual->getLeftPlace( ) != 0 ) {
					flag = false;

					if ( VVERBOSE ) {
						std::cout << "Sasiad ma flage true i nie ma wolnego miejsca, ale w actual jest wolne miejsce." << std::endl;
					}

					if ( dir & 0x01 ) {
						++neighbour;
					} else {
						--neighbour;
					}

					while ( neighbour != actual ) {

						if ( false == containerOrganized[neighbour->getIndex( )] && neighbour->getLeftPlace( ) != 0 ) {

							counter += containerSet->swapBlockFreeSpace( neighbour, actual, color );
							if ( VVERBOSE ) {
								std::cout << "Znalazlem kandydata do zamiany." << std::endl;
							}
							flag = true;
							break;

						} else if ( false == containerOrganized[neighbour->getIndex( )] && nullptr != neighbour->checkIsColorPresent( colorOrganizeMap ) ) {

							tmpColorPointer = neighbour->checkIsColorPresent( colorOrganizeMap );
							counter += containerSet->swapBlocksWithFreeSpace( actual, actual, color, neighbour, *tmpColorPointer );
							if ( VVERBOSE ) {
								std::cout << "Znalazlem kandydata do zamiany." << std::endl;
							}
							flag = true;
							break;

						} else if ( true == containerOrganized[neighbour->getIndex( )] && neighbour->getLeftPlace( ) != 0 ) {

							tmpIter = neighbour;

							if ( dir & 0x01 ) {
								++tmpIter;
							} else {
								--tmpIter;
							}

							while ( tmpIter != actual ) {
								tmpColorPointer = tmpIter->checkIsColorPresent( colorOrganizeMap );
								if ( false == containerOrganized[tmpIter->getIndex( )] && nullptr != tmpColorPointer ) {
									counter += containerSet->swapBlockFreeSpace( neighbour, tmpIter, *tmpColorPointer );
									counter += containerSet->swapBlockFreeSpace( tmpIter, actual, color );
									flag = true;
									if ( VVERBOSE ) {
										std::cout << "Znalazlem kandydata do zamiany" << std::endl;
									}
									break;
								}

								if ( dir & 0x01 ) {
									++tmpIter;
								} else {
									--tmpIter;
								}

							}

							if ( flag ) {
								break;
							}

						}

						if ( dir & 0x01 ) {
							++neighbour;
						} else {
							--neighbour;
						}

					}

					if ( flag == false ) {
						std::cerr << "Nie ma mozliwosci rozlozenia [3]" << std::endl;
						exit( 1 );
					}

					flag = false;

					if ( VVERBOSE ) {
						containerSet->showInfo( );
					}
					continue;
				}

				// Sasiad ma flage true i nie ma wolnego miejsca, w actual tez nie ma wolnego miejsca.
				if ( neighbour->getLeftPlace( ) == 0 && actual->getLeftPlace( ) == 0 ) {
					flag = false;

					if ( VVERBOSE ) {
						std::cout << "Sasiad ma flage true i nie ma wolnego miejsca, w aktualnie rozpatrywanym pojemniku tez nie ma wolnego miejsca." << std::endl;
					}

					// Znalezienie wolnego miejsca.
					freeSpace = actual;

					if ( dir & 0x01 ) {
						++freeSpace;
					} else {
						--freeSpace;
					}

					while ( freeSpace != actual ) {
						if ( freeSpace->getLeftPlace( ) > 0 ) {
							break;
						}

						if ( dir & 0x01 ) {
							++freeSpace;
						} else {
							--freeSpace;
						}

					}

					if ( freeSpace == actual ) {
						std::cerr << "Nie znaleziono wolnego miejsca !" << std::endl;
						return;
					}

					if ( dir & 0x01 ) {
						++neighbour;
					} else {
						--neighbour;
					}

					while ( neighbour != actual ) {

						if ( false == containerOrganized[neighbour->getIndex( )] && neighbour->getLeftPlace( ) != 0 ) {

							counter += containerSet->swapBlockFreeSpace( neighbour, actual, color );
							if ( VVERBOSE ) {
								std::cout << "Znalazlem kandydata do zamiany" << std::endl;
							}
							flag = true;
							break;

						} else if ( false == containerOrganized[neighbour->getIndex( )] && nullptr != neighbour->checkIsColorPresent( colorOrganizeMap ) ) {
							
							tmpColorPointer = neighbour->checkIsColorPresent( colorOrganizeMap );
							counter += containerSet->swapBlocksWithFreeSpace( freeSpace, actual, color, neighbour, *tmpColorPointer );
							if ( VVERBOSE ) {
								std::cout << "Znalazlem kandydata do zamiany" << std::endl;
							}
							flag = true;
							break;

						} else if ( true == containerOrganized[neighbour->getIndex( )] && neighbour->getLeftPlace( ) != 0 ) {

							tmpIter = neighbour;

							if ( dir & 0x01 ) {
								++tmpIter;
							} else {
								--tmpIter;
							}

							while ( tmpIter != actual ) {
								tmpColorPointer = tmpIter->checkIsColorPresent( colorOrganizeMap );
								if ( false == containerOrganized[tmpIter->getIndex( )] && nullptr != tmpColorPointer ) {

									counter += containerSet->swapBlockFreeSpace( neighbour, tmpIter, *tmpColorPointer );
									counter += containerSet->swapBlockFreeSpace( tmpIter, actual, color );
									if ( VVERBOSE ) {
										std::cout << "Znalazlem kandydata do zamiany" << std::endl;
									}
									flag = true;
									break;
								}
							
								if ( dir & 0x01 ) {
									++tmpIter;
								} else {
									--tmpIter;
								}

							}

							if ( flag ) {
								break;
							}
						}

						if ( dir & 0x01 ) {
							++neighbour;
						} else {
							--neighbour;
						}
					}

					if ( flag == false ) {
						std::cerr << "Nie ma mozliwosci rozlozenia [4]" << std::endl;
						exit( 1 );
					}

					flag = false;

					if ( VVERBOSE ) {
						containerSet->showInfo( );
					}
					continue;
				}

			}

		}

		if ( CONTAINERVERBOSE ) {
			std::cout << std::endl << "Kolor numer " << color.getColor( ) << " w pojemniku o indeksie " << actual->getIndex( ) << " uporzadkowany..." << std::endl;
			containerSet->showInfo( );
		}

		containerOrganized[actual->getIndex( )] = true;
		actual = containerSet->getMaxiumWithColor( color );
		neighbour = actual;
		
		if ( dir & 0x01 ) {
			++neighbour;
		} else {
			--neighbour;
		}

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
