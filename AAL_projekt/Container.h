#ifndef CONTAINER_H
#define CONTAINER_H

#include <iostream>
#include <vector>

#include "Block.h"

class Container
{
private:
	std::vector<Block*> includedBlocks;
public:
	Container(std::vector<unsigned int> & included);
	~Container();

};

#endif 