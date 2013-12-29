/*
 * Projekt z AAL.
 *
 * Przemyslaw Piorkowski.
 */

//podstawowe komunikaty - pokazanie na poczatku i na koncu wygladu pojemnikow.
bool VERBOSE = false;
// pokazywanie w kazdej fazie pojemnikow z opisem dzialan
bool VVERBOSE = false;
// pokazywanie opisu kazdego wykonywanego ruchu.
bool VVVERBOSE = false;
// pokazywanie stanu po uporzadkowaniu kazdego pojemnika konkretnym kolorem
bool CONTAINERVERBOSE = false;
// zapisanie wygladu pojemnika do pliku.
bool FILEVERBOSE = false;
// zrodlo danych w pliku.
bool FILE_SOURCE = false;
// zrodlo danych z generatora.
bool RANDOM_SOURCE = false;
// help
bool HELP = false;

#include <iostream>
#include <stdlib.h>
#include "Interpreter.h"
#include "Container.h"
#include "ContainerSet.h"
#include "CycleCounter.h"
#include "DataGenerator.h"

/* Naglowki plikow z algorytmami rozwiazan */
#include "ContainersSweep.h"
#include "SearchTree.h"
#include "SelectionSort.h"

/*
 * Wejscie do programu.
 */
int main(int argc, char* argv[])
 {
	char * sourceFileName;
	char * destinyFileName;
	unsigned int numberOfContainers = 0;
	unsigned int minCapacity = 0;
	unsigned int maxCapacity = 0;
	float fillingDegree = 0.0;
	unsigned int colorsNumber = 0;


	for ( int i = 1; i < argc; ++i ) {
		if ( strcmp (argv[i] , "-v") == 0 ) {
			VERBOSE = true;
			continue;
		}
		if ( strcmp(argv[i] , "-vv") == 0) {
			VERBOSE = true;
			VVERBOSE = true;
			CONTAINERVERBOSE = true;
			continue;
		}
		if ( strcmp(argv[i] , "-vvv") == 0) {
			VERBOSE = true;
			VVERBOSE = true;
			VVVERBOSE = true;
			continue;
		}
		if ( strcmp(argv[i] , "-fv") == 0) {
			FILEVERBOSE = true;
			destinyFileName = argv[++i];
			continue;
		}
		if ( strcmp( argv[i], "-cv" ) == 0 ) {
			CONTAINERVERBOSE = true;
			continue;
		}
		if ( strcmp(argv[i] , "-f") == 0) {
			FILE_SOURCE = true;
			sourceFileName = argv[++i];
			continue;
		}
		if ( strcmp(argv[i],"-r" ) == 0) {
			RANDOM_SOURCE = true;
			numberOfContainers = atoi( argv[++i] );
			minCapacity = atoi( argv[++i] );
			maxCapacity = atoi( argv[++i] );
			fillingDegree = atof( argv[++i] );
			colorsNumber = atoi( argv[++i] );
			continue;
		}

		HELP = true;
		break;
	}

	if ( HELP ) {
		std::cout << "HELP" << std::endl;
		std::cout << std::endl;
		std::cout << "OPCJE : " << std::endl;
		std::cout << " -f   wybranie jako zrodla danych pliku.Po tej fladze powinna znalezc sie nazwa" << std::endl;
		std::cout << "      pliku z danymi." << std::endl;
		std::cout << " -r   wybranie jako zrodla danych generatora danych losowych. Dane jakie mozna" << std::endl;
		std::cout << "      podawac generatorowi to w kolejnosci: [LICZBA_POJEMNIKOW] [MIN_POJEMNOSC]" << std::endl;
		std::cout << "      [MAX_POJEMNOSC] [STOPIEN_ZAPELNIENIA] [LICZBA_KOLOROW]." << std::endl;
		std::cout << " -v   pokazywanie podstawowych komunikatow- wyglad zbioru pojemnikow przed i po" << std::endl;
		std::cout << "      zastosowaniu algorytmu." << std::endl;
		std::cout << " -vv  pokazywanie rozszezonych komunikatow - stan pojemnikow podczas kazdej" << std::endl;
		std::cout << "      decyzji, po oczyszczeniu pojemnika. Wyswietla tez to co opcja -v i -cv." << std::endl;
		std::cout << " -vvv najdokladniejszy poziom komunikatow - wysiwetlana jest wiadomosc przy" << std::endl;
		std::cout << "      przenoszeniu kazdego klocka pomiedzy pojemnikami. Wyswietla tez to" << std::endl;
		std::cout << "      co -vv i -v." << std::endl;
		std::cout << " -vc  wyswietlanie komuniaktu o wygladzie zbioru pojemnikow po uporzadkowaniu" << std::endl;
		std::cout << "      kazdego pojemnika kolorem." << std::endl;
		std::cout << " -fv  opcja zapisania wygladu pojemnika do pliku przed rozpoczeciem" << std::endl;
		std::cout << "      uporzadkowywania zbioru pojemnikow. Po tej opcji powinna znalezc sie " << std::endl;
		std::cout << "      nazwa pliku w ktorym maja zostac zapisane dane." << std::endl;
		
		#ifdef _WIN32
		system( "PAUSE" );
		#endif
		return 0;
	}

	if ( FILE_SOURCE ) {
		Interpreter interpreter( sourceFileName );
		CycleCounter cycleCounter;

		if ( interpreter.verifyGatheredInfo( ) && VERBOSE ) {
			interpreter.showGatheredInfo( );
		}

		ContainerSet containerSet( interpreter.getConatinersCapacity( ), interpreter.getIncludingList( ), interpreter.getColorsNumber( ) );
		ContainersSweep < SelectionSort> sweep( containerSet );

		if ( VERBOSE ) {
			containerSet.showInfo( );
		}

		if ( FILEVERBOSE ) {
			containerSet.showToFile( destinyFileName );
		}

		cycleCounter.startCounting( );
		sweep.solveProblem( );
		cycleCounter.stopCounting( );

		if ( VERBOSE ) {
			std::cout << std::endl << "After solving with " << sweep.getCounter( ) << " moves ... " << std::endl;
			sweep.showContainerSet( );
		}

		std::cout << std::endl << "Moves : " << sweep.getCounter( ) << " state : " << sweep.getState( ) << std::endl;
		std::cout << std::endl << "Number of cycles : " << cycleCounter.getNumerOfCycles( ) << std::endl;
	}

	if ( RANDOM_SOURCE ) {
		DataGenerator generator( numberOfContainers, minCapacity, maxCapacity, fillingDegree, colorsNumber );
		generator.generateData( );
		CycleCounter cycleCounter;
		ContainerSet containerSet( generator.getConatinersCapacity( ), generator.getIncludingList( ), generator.getColorsNumber( ) );
		ContainersSweep<SelectionSort> sweep( containerSet );

		if ( VERBOSE ) {
			containerSet.showInfo( );
		}

		if ( FILEVERBOSE ) {
			containerSet.showToFile( destinyFileName );
		}

		cycleCounter.startCounting( );
		sweep.solveProblem( );
		cycleCounter.stopCounting( );

		if ( VERBOSE ) {
			std::cout << std::endl << "After solving with " << sweep.getCounter( ) << " moves ... " << std::endl;
			sweep.showContainerSet( );
		}

		std::cout << std::endl << "Moves : " << sweep.getCounter( ) << " state : " << sweep.getState( ) << std::endl;
		std::cout << std::endl << "Number of cycles : " << cycleCounter.getNumerOfCycles( ) << std::endl;
	}


	//ContainerSet::iterator firstBlock = containerSet.begin();
	//++firstBlock;
	//++firstBlock;
	//ContainerSet::iterator secondBlock = containerSet.begin();
	//++secondBlock;
	//++secondBlock;
	//++secondBlock;
	//++secondBlock;
	//++secondBlock;
	//ContainerSet::iterator freeSpace = containerSet.begin();
	//freeSpace++;
	//freeSpace++;
	//freeSpace++;
	//freeSpace++;
	//freeSpace++;
	//Color col1(3);
	//Color col2(1);

	//containerSet.swapBlockFreeSpace( firstBlock, secondBlock, col2 );
	///containerSet.showInfo( );
	//ContainersSweep<SelectionSort> sweep(containerSet);

	//cycleCounter.startCounting();
    //sweep.solveProblem();
    //cycleCounter.startCounting();

    //ContainerSet containerSet1(generator.getConatinersCapacity(), generator.getIncludingList(), generator.getColorsNumber());
	//containerSet1.showInfo();


    #ifdef _WIN32
	system("PAUSE");
	#endif

	return 0;
}

