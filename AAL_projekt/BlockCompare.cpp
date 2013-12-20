/*
 * BlockCompare.cpp
 *
 *  Created on: 20 gru 2013
 *      Author: przemek
 */

#include "BlockCompare.h"

BlockCompare::BlockCompare() { }

BlockCompare::~BlockCompare() { }

/*
 * Operator realizujacy porownanie dwoch obiektow klasy Block.
 */
bool BlockCompare::operator ()(const Block & rhs, const Block & lhs) const
{
	return rhs < lhs;
}
