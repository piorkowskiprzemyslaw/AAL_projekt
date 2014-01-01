#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <list>


#include "ContainerSet.h"

/**
 * Klasa opisujaca wezel drzewa przeszukiwania.
 */
class Node
{
private:
	// wskaznik na zbior pojemnikow reprezentujacy ten stan
	ContainerSet* containerSet;
	// lista dzieci tego stanu
	std::list<Node *> children;
	// wskaznik na ojca tego stanu.
	Node * parent;
	// stan pojemnikow  w tym wezle.
	bool state;
	// liczba ruchow pozwalajaca dojsc do tego stanu.
	const unsigned int moves;

public:
	Node( );
	Node( const ContainerSet & containerSet );
	Node( ContainerSet * containerSet, Node * parent , const unsigned int moves);
	~Node( );
	void addChildrens( );
	bool findACopy( const ContainerSet * containerSet ) const;
	void findSolution( std::list<Node *> & solutions );
	void showInfo( ) const;
	unsigned int getMoves( ) const;
	bool getState( ) const;
};

#endif
