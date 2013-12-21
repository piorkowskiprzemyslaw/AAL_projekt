/*
 * ColorCompare.cpp
 *
 *  Created on: 21 gru 2013
 *      Author: Przemyslaw Piorkowski
 */

#include "ColorCompare.h"

ColorCompare::ColorCompare() {}

ColorCompare::~ColorCompare() {}

bool ColorCompare::operator ()(const Color & rhs, const Color & lhs) const
{
	return rhs.getColor() < lhs.getColor();
}
