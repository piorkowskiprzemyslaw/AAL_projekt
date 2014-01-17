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
// komunikowanie tylko o ukonczeniu porzadkowania jakiegos koloru.
bool COLORVERBOSE = false;
// zrodlo danych w pliku.
bool FILE_SOURCE = false;
// zrodlo danych z generatora.
bool RANDOM_SOURCE = false;
// mierzenie czasu
bool TIMEMEASURE = false;
// mierzenie cykli procesora
bool CYCLEMEASURE = false;
// one dir container sweep
bool ONEDIRSWEEP = false;
// two dir containers sweep
bool TWODIRSWEEP = false;
// search tree metchod
bool SEARCHTREE = false;
// help
bool HELP = false;

#include <iostream>
#include <stdlib.h>
#include "Interpreter.h"
#include "Container.h"
#include "ContainerSet.h"
#include "CycleCounter.h"
#include "TimeCounter.h"
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
	CycleCounter cycleCounter;
	TimeCounter timeCounter;


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
		if ( strcmp( argv[i], "-colv" ) == 0 ) {
			COLORVERBOSE = true;
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
			fillingDegree = (float) atof( argv[++i] );
			colorsNumber = atoi( argv[++i] );
			continue;
		}
		if ( strcmp( argv[i], "-mt" ) == 0 ) {
			TIMEMEASURE = true;
			continue;
		}
		if ( strcmp( argv[i], "-mc" ) == 0 ) {
			CYCLEMEASURE = true;
			continue;
		}
		if ( strcmp( argv[i], "-cso" ) == 0 ) {
			ONEDIRSWEEP = true;
			continue;
		}
		if ( strcmp( argv[i], "-cst" ) == 0 ) {
			TWODIRSWEEP = true;
			continue;
		}
		if ( strcmp( argv[i], "-st" ) == 0 ) {
			SEARCHTREE = true;
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
		std::cout << " -cv  wyswietlanie komuniaktu o wygladzie zbioru pojemnikow po uporzadkowaniu" << std::endl;
		std::cout << "      kazdego pojemnika kolorem." << std::endl;
		std::cout << " -colv wyswietlenie komunikatu o uporzadkowaniu konretnego koloru oraz" << std::endl;
		std::cout << "      informacji ile kolorow do uporzadkowania pozostalo." << std::endl;
		std::cout << " -fv  opcja zapisania wygladu pojemnika do pliku przed rozpoczeciem" << std::endl;
		std::cout << "      uporzadkowywania zbioru pojemnikow. Po tej opcji powinna znalezc sie " << std::endl;
		std::cout << "      nazwa pliku w ktorym maja zostac zapisane dane." << std::endl;
		std::cout << " -mt  mierzenie czasu wykonania algorytmu za pomoca zegara." << std::endl;
		std::cout << " -mc  mierzenie czasu wykonania algorytmu za pomoca odczytu z licznika" << std::endl;
		std::cout << "      instrukcji procesora." << std::endl;
		std::cout << " -cso rozwiazanie metoda omiatania w jedna strone." << std::endl;
		std::cout << " -cst rozwiazanie metoda omiatania w dwie strony." << std::endl;
		std::cout << " -st  rozwiazanie metoda drzewa przeszukiwan." << std::endl;

		#ifdef _WIN32
		system( "PAUSE" );
		#endif
		return 0;
	}

	if ( FILE_SOURCE ) {
		Interpreter interpreter( sourceFileName );

		if ( interpreter.verifyGatheredInfo( ) && VERBOSE ) {
			interpreter.showGatheredInfo( );
		}

		ContainerSet containerSet( interpreter.getConatinersCapacity( ), interpreter.getIncludingList( ), interpreter.getColorsNumber( ) );
		ContainersSweep < SelectionSort> sweep( containerSet );
		SearchTree searchTree( containerSet );

		if ( VERBOSE ) {
			containerSet.showInfo( );
		}

		if ( FILEVERBOSE ) {
			containerSet.showToFile( destinyFileName );
		}

		if ( CYCLEMEASURE ) {
			cycleCounter.startCounting( );
		}
		if ( TIMEMEASURE ) {
			timeCounter.start( );
		}

		if ( ONEDIRSWEEP ) {
			sweep.solveProblem( oneDir );
		}
		if ( TWODIRSWEEP ) {
			sweep.solveProblem( twoDir );
		}
		if ( SEARCHTREE ) {
			searchTree.solve( );
		}

		if ( CYCLEMEASURE ) {
			cycleCounter.stopCounting( );
		}
		if ( TIMEMEASURE ) {
			timeCounter.stop( );
		}

		if ( VERBOSE && ( ONEDIRSWEEP || TWODIRSWEEP ) ) {
			std::cout << std::endl << "After solving with " << sweep.getCounter( ) << " moves ... " << std::endl;
			sweep.showContainerSet( );
		}

		if ( VERBOSE && SEARCHTREE ) {
			std::cout << std::endl << "After solving with " << searchTree.getMoves( ) << " moves... " << std::endl;
			searchTree.showSolution( );
		}

		if ( ONEDIRSWEEP || TWODIRSWEEP ) {
			std::cout << std::endl << "Moves : " << sweep.getCounter( ) << " state : " << sweep.getState( ) << std::endl;
		}
		if ( SEARCHTREE ) {
			std::cout << std::endl << "Moves : " << searchTree.getMoves( ) << " state : " << searchTree.getState( ) << std::endl;
		}

		if ( CYCLEMEASURE ) {
			std::cout << std::endl << "Number of cycles : " << cycleCounter.getNumerOfCycles( ) << " [x10^6]" << std::endl;
		}
		if ( TIMEMEASURE ) {
			std::cout << std::endl << "Elapsed time : " << timeCounter.getMeasuredTime( ) << " [ms]" << std::endl;
		}
	}

	if ( RANDOM_SOURCE ) {
		DataGenerator generator( numberOfContainers, minCapacity, maxCapacity, fillingDegree, colorsNumber );
		generator.generateData( );
		ContainerSet containerSet( generator.getConatinersCapacity( ), generator.getIncludingList( ), generator.getColorsNumber( ) );
		ContainersSweep<SelectionSort> sweep( containerSet );
		SearchTree searchTree( containerSet );

		if ( VERBOSE ) {
			containerSet.showInfo( );
		}

		if ( FILEVERBOSE ) {
			containerSet.showToFile( destinyFileName );
		}

		if ( CYCLEMEASURE ) {
			cycleCounter.startCounting( );
		}
		if ( TIMEMEASURE ) {
			timeCounter.start( );
		}

		if ( ONEDIRSWEEP ) {
			sweep.solveProblem( oneDir );
		}
		if ( TWODIRSWEEP ) {
			sweep.solveProblem( twoDir );
		}
		if ( SEARCHTREE ) {
			searchTree.solve( );
		}

		if ( CYCLEMEASURE ) {
			cycleCounter.stopCounting( );
		}
		if ( TIMEMEASURE ) {
			timeCounter.stop( );
		}

		if ( VERBOSE && ( ONEDIRSWEEP || TWODIRSWEEP ) ) {
			std::cout << std::endl << "After solving with " << sweep.getCounter( ) << " moves ... " << std::endl;
			sweep.showContainerSet( );
		}

		if ( VERBOSE && SEARCHTREE ) {
			std::cout << std::endl << "After solving with " << searchTree.getMoves( ) << " moves... " << std::endl;
			searchTree.showSolution( );
		}

		if ( ONEDIRSWEEP || TWODIRSWEEP ) {
			std::cout << std::endl << "Moves : " << sweep.getCounter( ) << " state : " << sweep.getState( ) << std::endl;
		}
		if ( SEARCHTREE ) {
			std::cout << std::endl << "Moves : " << searchTree.getMoves( ) << " state : " << searchTree.getState( ) << std::endl;
		}

		if ( CYCLEMEASURE ) {
			std::cout << std::endl << "Number of cycles : " << cycleCounter.getNumerOfCycles( ) << " [x10^6]" << std::endl;
		}
		if ( TIMEMEASURE ) {
			std::cout << std::endl << "Elapsed time : " << timeCounter.getMeasuredTime( ) << "[ms]" << std::endl;
		}
	}

    #ifdef _WIN32
	system("PAUSE");
	#endif

	return 0;
}

