#ifndef SEARCHTREE_H
#define SEARCHTREE_H

#include <iostream>
#include <list>
#include "Node.h"
#include "ContainerSet.h"

/*
 * Klasa realizująca algorytm przeszukiwania zbioru rozwiązań za pomocą drzewa binarnego.
 *
 */
class SearchTree
{
private:
	// Korzen drzewa przeszukiwan - aytuacja wejsciowa.
	Node * root;
	// Znalezione najlepsze rozwiazanie.
	Node * bestSolution;
	// Lista z wezlami reprezentujacymi rozwiazania.
	std::list<Node *> solutions;

	void findTheBestSolution( );
public:
    SearchTree();
	SearchTree( const ContainerSet & containerSet );
    ~SearchTree();
    void solve();
	unsigned int getMoves( ) const;
	bool getState( ) const;
	void showSolution( ) const;
};

#endif // SEARCHTREE_H
