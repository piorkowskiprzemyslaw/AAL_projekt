#include "CycleCounter.h"

//  Windows
#ifdef _WIN32
#include <intrin.h>
uint64_t getCounter(){
	return __rdtsc();
}
//  Linux/GCC
#else
uint64_t getCounter(){
	unsigned int lo, hi;
	__asm__ __volatile__("rdtsc" : "=a" (lo), "=d" (hi));
	return ((uint64_t)hi << 32) | lo;
}
#endif

/* 
 * Stworzenie nowego zegara.
 */
CycleCounter::CycleCounter() : startTime(0), stopTime(0) { }

/*
 * Zniszczenie zegara.
 */
CycleCounter::~CycleCounter() { }

/*
 * Rozpoczêcie liczenia cykli.
 */
void CycleCounter::startCounting()
{
	startTime = getCounter();
}

/*
 * Zakonczenie liczenia cykli.
 */
void CycleCounter::stopCounting()
{
	stopTime = getCounter();
}

/*
 * Pobranie liczby cykli.
 */
uint64_t CycleCounter::getNumerOfCycles()
{
	return stopTime - startTime;
}

/*
 * Resetowanie licznika.
 */
void CycleCounter::resetCounter()
{
	startTime = 0;
	stopTime = 0;
}