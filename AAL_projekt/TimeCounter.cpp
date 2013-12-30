#include "TimeCounter.h"


TimeCounter::TimeCounter( ) { }


TimeCounter::~TimeCounter( ) { }

/**
 * Rozpoczyna pomiar.
 */
void TimeCounter::start( )
{
	startTime = clock( );
}

/**
 * Konczy pomiar.
 */
void TimeCounter::stop( )
{
	stopTime = clock( );
}

/**
 * Pobiera ilosc czasu ktory uplynal. Wynik w ms.
 */
double TimeCounter::getMeasuredTime( )
{
	double diff = stopTime - startTime;
	double diffms = ( diff ) / ( CLOCKS_PER_SEC / 1000 );
	return diffms;
}