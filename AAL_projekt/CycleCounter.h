#ifndef CYCLECOUNTER_H
#define CYCLECOUNTER_H

#include <iostream>
#include <stdint.h>

/*
 * Klasa reprezentuj¹ca zegar cykli procesora.
 */
class CycleCounter
{
private:
	uint64_t startTime;
	uint64_t stopTime;
public:
	CycleCounter();
	~CycleCounter();
	void startCounting();
	void stopCounting();
	uint64_t getNumerOfCycles();
	void resetCounter();
};

#endif

