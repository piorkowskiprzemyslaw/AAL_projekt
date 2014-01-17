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
	unsigned int falseNeighbor( const ContainerSet::iterator & actual,
								const ContainerSet::iterator & neighbor, const Color & color );

	unsigned int falseNeighborNoPlace( const ContainerSet::iterator & actual,
									   const ContainerSet::iterator & neighbor, const Color & color,
									   const std::map<Color, bool, ColorCompare> & table,
									   const int dir, const std::vector<bool> & containerOrganized );

	unsigned int falseNeighborNoPlaceNoPlace( const ContainerSet::iterator & actual,
											  const ContainerSet::iterator & neighbor,
											  const Color & color, const std::map<Color,
											  bool, ColorCompare> & table, const int dir,
											  const std::vector<bool> & containerOrganized );

	unsigned int trueNeighbor( const ContainerSet::iterator & actual,
							   const ContainerSet::iterator & neighbor, const Color & color,
							   const std::map<Color, bool, ColorCompare> & table, const int dir,
							   const std::vector<bool> & containerOrganized );

	unsigned int trueNeighborNoPlace( const ContainerSet::iterator & actual,
									  const ContainerSet::iterator & neighbor,
									  const Color & color,
									  const std::map<Color, bool, ColorCompare> & table,
									  const int dir,
									  const std::vector<bool> & containerOrganized );

	unsigned int trueNeighborNoPlaceNoPlace( const ContainerSet::iterator & actual,
											 const ContainerSet::iterator & neighbor,
											 const Color & color,
											 const std::map<Color, bool, ColorCompare> & table,
											 const int dir,
											 const std::vector<bool> & containerOrganized );

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
bool ContainersSweep<T>::checkPreconditions( )
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

	std::cout << "Solving problem with ContainersSweep";
	if ( mode == oneDir ) std::cout << "OneDirection." << std::endl;
	if ( mode == twoDir ) std::cout << "TwoDirections." << std::endl;
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
 * Prywatna metoda rozwiazyjaca przypadek gdy :
 * sasiad ma flage false i ma wolne miejsce.
 */
template<typename T>
unsigned int ContainersSweep<T>::falseNeighbor( const ContainerSet::iterator & actual,
												const ContainerSet::iterator & neighbor,
												const Color & color )
{
	if ( !containerSet->checkForNeighborhood( actual, neighbor ) ) {
		std::cerr << "Proba przeniesienia klocka wbrew zasadom." << std::endl;
		exit( 1 );
	}

	actual->moveBlock( color, *neighbor );

	if ( VVERBOSE ) {
		std::cout << "Sasiad ma falge flase i ma wolne miejsce" << std::endl;
		containerSet->showInfo( );
	}

	return 1;
}

/**
 * Metoda rozpatrujaca przypadek gdy :
 * Sasiad ma flage false i nie ma wolnego miejsca ale w actual jest wolne miejsce.
 */
template<typename T>
unsigned int ContainersSweep<T>::falseNeighborNoPlace( const ContainerSet::iterator & actual,
													   const ContainerSet::iterator & neighbor,
													   const Color & color,
													   const std::map<Color, bool, ColorCompare> & colorOrganizeMap,
													   const int dir, const std::vector<bool> & containerOrganized )
{
	// Licznik ruchow.
	unsigned int counter = 0;
	bool flag = false;
	Color * tmpColorPointer = nullptr;
	ContainerSet::iterator tmpNeighbor( neighbor );
	ContainerSet::iterator tmpIter( neighbor );

	if ( VVERBOSE ) {
		std::cout << "Sasiad ma flage false i nie ma wolnego miejsca ale w actual jest wolne miejsce." << std::endl;
	}

	tmpColorPointer = neighbor->checkIsColorPresent( colorOrganizeMap );
	if ( tmpColorPointer != nullptr ) {
		// Nie rozpatrywany kolor znajduje sie u sąsiada. Wykonanie swapa.
		if ( VVERBOSE ) {
			std::cout << "Prawy sasiad ma kolor jeszcze nie rozpatrywany." << std::endl;
		}

		neighbor->moveBlock( *tmpColorPointer, *actual );
		actual->moveBlock( color, *neighbor );
		counter += 2;
	} else {
		// Nie rozpatrywany kolor nie znajduje sie u sasiada, szukamy dalej w kierunku dir.
		if ( VVERBOSE ) {
			std::cout << "Sasiad nie ma koloru jeszcze nie rozpatrywanego, szukam dalej w kierunku dir." << std::endl;
		}

		if ( dir & 0x01 ) ++tmpNeighbor;
		else  --tmpNeighbor;

		while ( tmpNeighbor != actual ) {
			if ( true == containerOrganized[tmpNeighbor->getIndex( )] && tmpNeighbor->getLeftPlace( ) != 0 ) {
				tmpIter = tmpNeighbor;
				if ( dir & 0x01 ) ++tmpIter;
				else  --tmpIter;

				while ( tmpIter != actual ) {
					tmpColorPointer = tmpIter->checkIsColorPresent( colorOrganizeMap );
					if ( false == containerOrganized[tmpIter->getIndex( )] && tmpColorPointer != nullptr ) {
						counter += containerSet->swapBlockFreeSpace( tmpNeighbor, tmpIter, *tmpColorPointer );
						counter += containerSet->swapBlockFreeSpace( tmpIter, actual, color );
						flag = true;
						break;
					}

					if ( dir & 0x01 ) ++tmpIter;
					else  --tmpIter;
				}
				if ( flag ) break;
			} else if ( false == containerOrganized[tmpNeighbor->getIndex( )] && tmpNeighbor->getLeftPlace( ) != 0 ) {
				counter += containerSet->swapBlockFreeSpace( tmpNeighbor, actual, color );
				flag = true;
				break;
			} else if ( false == containerOrganized[tmpNeighbor->getIndex( )] && nullptr != tmpNeighbor->checkIsColorPresent( colorOrganizeMap ) ) {
				tmpColorPointer = tmpNeighbor->checkIsColorPresent( colorOrganizeMap );
				counter += containerSet->swapBlocksWithFreeSpace( actual, actual, color, tmpNeighbor, *tmpColorPointer );
				flag = true;
				break;
			}

			if ( dir & 0x01 ) ++tmpNeighbor;
			else  --tmpNeighbor;
		}

		if ( flag == false ) {
			std::cerr << "Nie ma mozliwosci rozlozenia [0]" << std::endl;
			exit( 1 );
		}
	}

	if ( VVERBOSE ) {
		containerSet->showInfo( );
	}

	return counter;
}

/**
 * Metoda realizujaca przypadek :
 * Sasaiad ma flage flase, nie ma wolnego miejsca, w akualnie rozpatrywanym pojemniku tez nie ma wolnego miejsca.
 */
template<typename T>
unsigned int ContainersSweep<T>::falseNeighborNoPlaceNoPlace( const ContainerSet::iterator & actual,
															  const ContainerSet::iterator & neighbor,
															  const Color & color,
															  const std::map<Color, bool, ColorCompare> & colorOrganizeMap,
															  const int dir,
															  const std::vector<bool> & containerOrganized )
{
	// Licznik.
	unsigned int counter = 0;
	ContainerSet::iterator freeSpace( actual );
	ContainerSet::iterator tmpNeighbor( neighbor );
	ContainerSet::iterator tmpIter( actual );
	bool flag = false;
	Color * tmpColorPointer = nullptr;

	if ( VVERBOSE ) {
		std::cout << "Prawy sasaiad ma flage flase, nie ma wolnego miejsca, w akualnie rozpatrywanym pojemniku tez nie ma wolnego miejsca." << std::endl;
	}

	// Znalezienie wolnego miejsca.
	if ( dir & 0x01 ) ++freeSpace;
	else  --freeSpace;

	while ( freeSpace != actual ) {
		if ( freeSpace->getLeftPlace( ) > 0 ) {
			break;
		}

		if ( dir & 0x01 ) ++freeSpace;
		else  --freeSpace;
	}

	tmpColorPointer = neighbor->checkIsColorPresent( colorOrganizeMap );
	if ( tmpColorPointer != nullptr ) {
		if ( VVERBOSE ) {
			std::cout << "Sasiad ma kolor jeszcze nie rozpatrywany. Wykonanie zamiany klockow miejscami." << std::endl;
		}
		counter += containerSet->swapBlocksWithFreeSpace( freeSpace, actual, color, neighbor, *tmpColorPointer );
	} else {
		// Nie rozpatrywany kolor nie znajduje sie u sasiada, szukamy dalej w kierunku dir.
		if ( VVERBOSE ) {
			std::cout << "Sasiad nie ma koloru jeszcze nie rozpatrywanego, szukam dalej." << std::endl;
		}

		if ( dir & 0x01 ) ++tmpNeighbor;
		else  --tmpNeighbor;

		while ( tmpNeighbor != actual ) {
			if ( true == containerOrganized[tmpNeighbor->getIndex( )] && tmpNeighbor->getLeftPlace( ) != 0 ) {

				tmpIter = tmpNeighbor;
				if ( dir & 0x01 ) ++tmpIter;
				else  --tmpIter;

				while ( tmpIter != actual ) {
					tmpColorPointer = tmpIter->checkIsColorPresent( colorOrganizeMap );
					if ( false == containerOrganized[tmpIter->getIndex( )] && tmpColorPointer != nullptr ) {
						counter += containerSet->swapBlockFreeSpace( tmpNeighbor, tmpIter, *tmpColorPointer );
						counter += containerSet->swapBlockFreeSpace( tmpIter, actual, color );
						flag = true;
						break;
					}
					if ( dir & 0x01 ) ++tmpIter;
					else  --tmpIter;
				}
				if ( flag )	break;
			} else if ( false == containerOrganized[tmpNeighbor->getIndex( )] && tmpNeighbor->getLeftPlace( ) != 0 ) {
				counter += containerSet->swapBlockFreeSpace( tmpNeighbor, actual, color );
				flag = true;
				break;
			} else if ( false == containerOrganized[tmpNeighbor->getIndex( )] && nullptr != tmpNeighbor->checkIsColorPresent( colorOrganizeMap ) ) {
				tmpColorPointer = tmpNeighbor->checkIsColorPresent( colorOrganizeMap );
				counter += containerSet->swapBlocksWithFreeSpace( freeSpace, actual, color, tmpNeighbor, *tmpColorPointer );
				flag = true;
				break;
			}

			if ( dir & 0x01 ) ++tmpNeighbor;
			else  --tmpNeighbor;
		}
		if ( flag == false ) {
			std::cerr << "Nie ma mozliwosci rozlozenia [1]" << std::endl;
			exit( 1 );
		}
	}

	if ( VVERBOSE ) {
		containerSet->showInfo( );
	}
	return counter;
}

/**
 * Metoda realizujaca przypadek :
 * Sasiad ma flage true i ma wolone miejsce.
 */
template <typename T>
unsigned int ContainersSweep<T>::trueNeighbor( const ContainerSet::iterator & actual,
											   const ContainerSet::iterator & neighbor,
											   const Color & color,
											   const std::map<Color, bool, ColorCompare> & colorOrganizeMap,
											   const int dir,
											   const std::vector<bool> & containerOrganized )
{
	// Licznik
	unsigned int counter = 0;
	// Zmienne pomocnicze.
	bool flag = false;
	ContainerSet::iterator tmpIter( neighbor );
	Color * tmpColorPointer;

	if ( VVERBOSE ) {
		std::cout << "Sasiad ma flage true i ma wolone miejsce." << std::endl;
	}

	if ( dir & 0x01 ) ++tmpIter;
	else  --tmpIter;

	while ( tmpIter != neighbor ) {
		tmpColorPointer = tmpIter->checkIsColorPresent( colorOrganizeMap );
		if ( nullptr != tmpColorPointer && false == containerOrganized[tmpIter->getIndex( )] ) {
			counter += containerSet->swapBlockFreeSpace( neighbor, tmpIter, *tmpColorPointer );
			counter += containerSet->swapBlockFreeSpace( tmpIter, actual, color );
			if ( VVERBOSE ) {
				std::cout << "Znalazlem kandydata do zamiany i wykonalem zamiane." << std::endl;
			}
			flag = true;
			break;
		}
		if ( dir & 0x01 ) ++tmpIter;
		else  --tmpIter;
	}

	// Jesli nie zostal znaleziony zaden kandydat do zamiany, to szukamy wolnego miejsca w
	// pojemnikach po dir stronie. Miejsce takie musi sie znalezc.
	if ( !flag ) {
		tmpIter = neighbor;
		if ( dir & 0x01 ) ++tmpIter;
		else  --tmpIter;

		while ( tmpIter != neighbor ) {
			if ( false == containerOrganized[tmpIter->getIndex( )] && tmpIter->getLeftPlace( ) != 0 ) {
				counter += containerSet->swapBlockFreeSpace( tmpIter, actual, color );
				flag = true;
				if ( VVERBOSE ) {
					std::cout << "Znalazlem kandydata z flaga false i wolnym miejscem, przenosze tam klocek." << std::endl;
				}
				break;
			}
			if ( dir & 0x01 ) ++tmpIter;
			else  --tmpIter;
		}
	}

	if ( flag == false ) {
		std::cerr << "Nie ma mozliwosci rozlozenia [2]" << std::endl;
		exit( 1 );
	}

	if ( VVERBOSE ) {
		containerSet->showInfo( );
	}

	return counter;
}

/**
 * Metoda rozpatrujaca przypadek :
 * Sasiad ma flage true i nie ma wolnego miejsca, ale w aktualnie rozpatrywanym
 * pojemniku jest wolne miejsce.
 */
template <typename T>
unsigned int ContainersSweep<T>::trueNeighborNoPlace( const ContainerSet::iterator & actual,
													  const ContainerSet::iterator & neighbor,
													  const Color & color,
													  const std::map<Color, bool, ColorCompare> & colorOrganizeMap,
													  const int dir,
													  const std::vector<bool> & containerOrganized )
{
	// Licznik
	unsigned int counter = 0;
	bool flag = false;
	ContainerSet::iterator tmpNeighbor( neighbor );
	ContainerSet::iterator tmpIter( neighbor );
	Color * tmpColorPointer = nullptr;

	if ( VVERBOSE ) {
		std::cout << "Prawy sąsiad ma flage true i nie ma wolnego miejsca, ale w aktualnie rozpatrywanym pojemniku jest wolne miejsce." << std::endl;
	}

	if ( dir & 0x01 ) ++tmpNeighbor;
	else  --tmpNeighbor;

	while ( tmpNeighbor != actual ) {

		if ( false == containerOrganized[tmpNeighbor->getIndex( )] && tmpNeighbor->getLeftPlace( ) != 0 ) {
			counter += containerSet->swapBlockFreeSpace( tmpNeighbor, actual, color );
			if ( VVERBOSE ) {
				std::cout << "Znalazlem kandydata do zamiany" << std::endl;
			}
			flag = true;
			break;
		} else if ( false == containerOrganized[tmpNeighbor->getIndex( )] && nullptr != tmpNeighbor->checkIsColorPresent( colorOrganizeMap ) ) {
			tmpColorPointer = tmpNeighbor->checkIsColorPresent( colorOrganizeMap );
			counter += containerSet->swapBlocksWithFreeSpace( actual, actual, color, tmpNeighbor, *tmpColorPointer );
			if ( VVERBOSE ) {
				std::cout << "Znalazlem kandydata do zamiany" << std::endl;
			}
			flag = true;
			break;
		} else if ( true == containerOrganized[tmpNeighbor->getIndex( )] && tmpNeighbor->getLeftPlace( ) != 0 ) {
			tmpIter = tmpNeighbor;
			if ( dir & 0x01 ) ++tmpIter;
			else  --tmpIter;

			while ( tmpIter != actual ) {
				tmpColorPointer = tmpIter->checkIsColorPresent( colorOrganizeMap );
				if ( false == containerOrganized[tmpIter->getIndex( )] && nullptr != tmpColorPointer ) {
					counter += containerSet->swapBlockFreeSpace( tmpNeighbor, tmpIter, *tmpColorPointer );
					counter += containerSet->swapBlockFreeSpace( tmpIter, actual, color );
					flag = true;
					if ( VVERBOSE ) {
						std::cout << "Znalazlem kandydata do zamiany" << std::endl;
					}
					break;
				}
				if ( dir & 0x01 ) ++tmpIter;
				else  --tmpIter;
			}
			if ( flag ) {
				break;
			}

		}

		if ( dir & 0x01 ) ++tmpNeighbor;
		else  --tmpNeighbor;
	}

	if ( flag == false ) {
		std::cerr << "Nie ma mozliwosci rozlozenia [3]" << std::endl;
		exit( 1 );
	}

	if ( VVERBOSE ) {
		containerSet->showInfo( );
	}
	return counter;
}

/**
 * Metoda rozpatrujaca przypadek :
 * Sasiad ma flage true i nie ma wolnego miejsca, w aktualnie rozpatrywanym pojemniku tez nie ma wolnego miejsca.
 */
template <typename T>
unsigned int ContainersSweep<T>::trueNeighborNoPlaceNoPlace( const ContainerSet::iterator & actual,
															 const ContainerSet::iterator & neighbor,
															 const Color & color,
															 const std::map<Color, bool, ColorCompare> & colorOrganizeMap,
															 const int dir,
															 const std::vector<bool> & containerOrganized )
{
	unsigned int counter = 0;
	bool flag = false;
	ContainerSet::iterator tmpNeighbor( neighbor );
	ContainerSet::iterator tmpIter( neighbor );
	ContainerSet::iterator freeSpace( actual );
	Color * tmpColorPointer = nullptr;

	if ( VVERBOSE ) {
		std::cout << "Prawy sasiad ma flage true i nie ma wolnego miejsca, w aktualnie rozpatrywanym pojemniku tez nie ma wolnego miejsca." << std::endl;
	}

	// Znalezienie wolnego miejsca.
	if ( dir & 0x01 ) ++freeSpace;
	else  --freeSpace;

	while ( freeSpace != actual ) {
		if ( freeSpace->getLeftPlace( ) > 0 ) {
			break;
		}

		if ( dir & 0x01 ) ++freeSpace;
		else  --freeSpace;
	}

	if ( dir & 0x01 ) ++tmpNeighbor;
	else  --tmpNeighbor;

	while ( tmpNeighbor != actual ) {
		if ( false == containerOrganized[tmpNeighbor->getIndex( )] && tmpNeighbor->getLeftPlace( ) != 0 ) {
			counter += containerSet->swapBlockFreeSpace( tmpNeighbor, actual, color );
			if ( VVERBOSE ) {
				std::cout << "Znalazlem kandydata do zamiany" << std::endl;
			}
			flag = true;
			break;
		} else if ( false == containerOrganized[tmpNeighbor->getIndex( )] && nullptr != tmpNeighbor->checkIsColorPresent( colorOrganizeMap ) ) {
			tmpColorPointer = tmpNeighbor->checkIsColorPresent( colorOrganizeMap );
			counter += containerSet->swapBlocksWithFreeSpace( freeSpace, actual, color, tmpNeighbor, *tmpColorPointer );
			if ( VVERBOSE ) {
				std::cout << "Znalazlem kandydata do zamiany" << std::endl;
			}
			flag = true;
			break;
		} else if ( true == containerOrganized[tmpNeighbor->getIndex( )] && tmpNeighbor->getLeftPlace( ) != 0 ) {
			tmpIter = tmpNeighbor;
			if ( dir & 0x01 ) ++tmpIter;
			else  --tmpIter;

			while ( tmpIter != actual ) {
				tmpColorPointer = tmpIter->checkIsColorPresent( colorOrganizeMap );
				if ( false == containerOrganized[tmpIter->getIndex( )] && nullptr != tmpColorPointer ) {
					counter += containerSet->swapBlockFreeSpace( tmpNeighbor, tmpIter, *tmpColorPointer );
					counter += containerSet->swapBlockFreeSpace( tmpIter, actual, color );
					if ( VVERBOSE ) {
						std::cout << "Znalazlem kandydata do zamiany" << std::endl;
					}
					flag = true;
					break;
				}
				if ( dir & 0x01 ) ++tmpIter;
				else  --tmpIter;
			}

			if ( flag ) {
				break;
			}
		}
		if ( dir & 0x01 ) ++tmpNeighbor;
		else  --tmpNeighbor;
	}

	if ( flag == false ) {
		std::cerr << "Nie ma mozliwosci rozlozenia [4]" << std::endl;
		exit( 1 );
	}

	if ( VVERBOSE ) {
		containerSet->showInfo( );
	}
	return counter;
}

/**
 * Metoda realizujaca sprzatanie konkretnego koloru w pudelkach omiatajac tylko w jedną strone.
 * Table to vector wartosci logicznych mowiacy czy kolor byl juz rozpatrywany.
 */
template <typename T>
void ContainersSweep<T>::organizeColorOneDir(const std::map<Color, bool, ColorCompare> & colorOrganizeMap, const Color & color)
{
	// Vector wartosci logicznych mowiacy czy kontener o indeksie rownym indeksowi w tablicy zostal juz uporzadkowany.
	std::vector<bool> containerOrganized(containerSet->size(),false);
	// Itearator na pojemnik z najwieksza liczba klockow koloru color
    ContainerSet::iterator actual = containerSet->getMaxiumWithColor(color);
	// Iterator na prawego sasiada
    ContainerSet::iterator rightNeighbor(actual);
    // liczba klockow koloru color w actual
	unsigned int blocksNumer = 0;

    // Omiatamy dopoki maksymalna ilosc znaleziona w pojemniku jest wieksza od 1.
	while ( actual->getColorMultiplicity( color ) > 1 ) {

		++rightNeighbor;
		blocksNumer = actual->getColorMultiplicity( color );
		if ( VVERBOSE ) {
			std::cout << "Licznosc koloru " << color.getColor( ) << " w pojemniku o indeksie " << actual->getIndex( ) << " = " << blocksNumer << std::endl;
		}

    	// Przekladamy wszystkie nadmiarowe kulki do prawych sasiadow.
    	for(size_t i = 1 ; i < blocksNumer ; ++i) {

    		// Przypadki kiedy prawy sasiad ma flage "false"
    		if(false == containerOrganized[rightNeighbor->getIndex()])
    		{
				//Prawy sasiad ma falge flase i ma wolne miejsce
				if ( rightNeighbor->getLeftPlace( ) != 0 ) {
					counter += falseNeighbor( actual, rightNeighbor, color );
					continue;
				}

				//Prawy sasiad ma flage false i nie ma wolnego miejsca ale u nas jest wolne miejsce
				if ( rightNeighbor->getLeftPlace( ) == 0 && actual->getLeftPlace( ) != 0 ) {
					counter += falseNeighborNoPlace( actual, rightNeighbor, color, colorOrganizeMap, 1, containerOrganized );
					continue;
				}

				//Prawy sasaiad ma flage flase, nie ma wolnego miejsca, w akualnie rozpatrywanym pojemniku tez nie ma wolnego miejsca.
				if(rightNeighbor->getLeftPlace() == 0 && actual->getLeftPlace() == 0) {
					counter += this->falseNeighborNoPlaceNoPlace( actual, rightNeighbor, color, colorOrganizeMap, 1, containerOrganized );
					continue;
				}

    		} else /* (true == containerOrganized[rightNeighbor->getIndex()]) */ {

    			// Prawy sasiad ma flage true i ma wolone miejsce.
				if ( rightNeighbor->getLeftPlace( ) != 0 ) {
					counter += trueNeighbor( actual, rightNeighbor, color, colorOrganizeMap, 1, containerOrganized );
    				continue;
    			}

    			// Prawy sąsiad ma flage true i nie ma wolnego miejsca, ale w aktualnie rozpatrywanym pojemniku jest wolne miejsce.
    			if(rightNeighbor->getLeftPlace() == 0 && actual->getLeftPlace() != 0) {
					counter += trueNeighborNoPlace( actual, rightNeighbor, color, colorOrganizeMap, 1, containerOrganized );
    				continue;
    			}

    			// Prawy sasiad ma flage true i nie ma wolnego miejsca, w aktualnie rozpatrywanym pojemniku tez nie ma wolnego miejsca.
    			if(rightNeighbor->getLeftPlace() == 0 && actual->getLeftPlace() == 0) {
					counter += trueNeighborNoPlaceNoPlace( actual, rightNeighbor, color, colorOrganizeMap, 1, containerOrganized );
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
    	rightNeighbor = actual;
    }
}

/**
 * Metoda realizujaca sprzatanie koloru color w pudelkach stosujac omiatanie w dwie strony.
 */
template<typename T>
void ContainersSweep<T>::organizeColorTwoDir( const std::map<Color, bool, ColorCompare> & colorOrganizeMap, const Color & color )
{
	// Vector wartosci logicznych mowiacy czy kontener o indeksie rownym indeksowi w tablicy zostal juz uporzadkowany.
	std::vector<bool> containerOrganized( containerSet->size( ), false );
	// Itearator na pojemnik z najwieksza liczba klockow koloru color
	ContainerSet::iterator actual = containerSet->getMaxiumWithColor( color );
	// Iterator na prawego sasiada
	ContainerSet::iterator neighbor( actual );
	// liczba klockow koloru color w actual
	unsigned int blocksNumer = 0;
	// Kierunek omiatania.
	int dir = 0;

	// Omiatamy dopoki maksymalna ilosc znaleziona w pojemniku jest wieksza od 1.
	while ( actual->getColorMultiplicity( color ) > 1 ) {

		blocksNumer = actual->getColorMultiplicity( color );
		if ( VVERBOSE ) {
			std::cout << "Licznosc koloru " << color.getColor( ) << " w pojemniku o indeksie " << actual->getIndex( ) << " = " << blocksNumer << std::endl;
		}

		// Przekladamy wszystkie nadmiarowe kulki do sasiadow.
		for ( size_t i = 1; i < blocksNumer; ++i ) {

			neighbor = actual;
			if ( i & 0x01 ) ++neighbor;
			else  --neighbor;

			// Przypadki kiedy prawy sasiad ma flage "false"
			if ( false == containerOrganized[neighbor->getIndex( )] ) {
				//Prawy sasiad ma falge flase i ma wolne miejsce
				if ( neighbor->getLeftPlace( ) != 0 ) {
					counter += falseNeighbor( actual, neighbor, color );
					continue;
				}

				//Prawy sasiad ma flage false i nie ma wolnego miejsca ale u nas jest wolne miejsce
				if ( neighbor->getLeftPlace( ) == 0 && actual->getLeftPlace( ) != 0 ) {
					counter += falseNeighborNoPlace( actual, neighbor, color, colorOrganizeMap, i, containerOrganized );
					continue;
				}

				//Prawy sasaiad ma flage flase, nie ma wolnego miejsca, w akualnie rozpatrywanym pojemniku tez nie ma wolnego miejsca.
				if ( neighbor->getLeftPlace( ) == 0 && actual->getLeftPlace( ) == 0 ) {
					counter += this->falseNeighborNoPlaceNoPlace( actual, neighbor, color, colorOrganizeMap, i, containerOrganized );
					continue;
				}

			} else /* (true == containerOrganized[neighbor->getIndex()]) */ {

				// Prawy sasiad ma flage true i ma wolone miejsce.
				if ( neighbor->getLeftPlace( ) != 0 ) {
					counter += trueNeighbor( actual, neighbor, color, colorOrganizeMap, i, containerOrganized );
					continue;
				}

				// Prawy sąsiad ma flage true i nie ma wolnego miejsca, ale w aktualnie rozpatrywanym pojemniku jest wolne miejsce.
				if ( neighbor->getLeftPlace( ) == 0 && actual->getLeftPlace( ) != 0 ) {
					counter += trueNeighborNoPlace( actual, neighbor, color, colorOrganizeMap, i, containerOrganized );
					continue;
				}

				// Prawy sasiad ma flage true i nie ma wolnego miejsca, w aktualnie rozpatrywanym pojemniku tez nie ma wolnego miejsca.
				if ( neighbor->getLeftPlace( ) == 0 && actual->getLeftPlace( ) == 0 ) {
					counter += trueNeighborNoPlaceNoPlace( actual, neighbor, color, colorOrganizeMap, i, containerOrganized );
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
