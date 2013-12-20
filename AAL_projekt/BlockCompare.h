/*
 * BlockCompare.h
 *
 *  Created on: 20 gru 2013
 *      Author: Przemysław Piórkowski
 */

#ifndef BLOCKCOMPARE_H_
#define BLOCKCOMPARE_H_

#include "Block.h"

/*
 * Klasa realizująca operator() do porównywania wartości dwóch obieków Block.
 */
class BlockCompare {
public:
	BlockCompare();
	~BlockCompare();
	bool operator()(const Block & rhs, const Block & lhs) const;
};

#endif /* BLOCKCOMPARE_H_ */
