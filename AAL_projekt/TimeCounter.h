#ifndef TIMECOUNTER_H
#define TIMECOUNTER_H

#include <iostream>
#include <ctime>

/**
 * Klasa sluzaca do pomiaru czasu.
 */
class TimeCounter
{
private:
	clock_t startTime;
	clock_t stopTime;
public:
	TimeCounter( );
	~TimeCounter( );
	void start( );
	void stop( );
	double getMeasuredTime( );
};

#endif
