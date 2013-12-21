/*
 * ColorCompare.h
 *
 *  Created on: 21 gru 2013
 *      Author: Przemyslaw Piorkowski
 */

#ifndef COLORCOMPARE_H_
#define COLORCOMPARE_H_

#include "Color.h"

class ColorCompare {
public:
	ColorCompare();
	~ColorCompare();
	bool operator()(const Color & rhs, const Color & lhs) const;
};

#endif /* COLORCOMPARE_H_ */
