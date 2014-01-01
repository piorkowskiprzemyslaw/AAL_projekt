#include "SearchTree.h"

/**
 * Domyslny konstrutkor
 */
SearchTree::SearchTree()
{
	root = nullptr;
	bestSolution = nullptr;
}

/**
 * Konstrutkor parametryczny.
 */
SearchTree::SearchTree( const ContainerSet & containerSet )
{
	root = new Node( containerSet );
	bestSolution = nullptr;
}

/**
 * Destrutkor
 */
SearchTree::~SearchTree()
{
	delete( root );
}

/** 
 * Rozawiazanie problemu.
 */
void SearchTree::solve( )
{
	std::cout << "Solving with SearchTree..." << std::endl;
	root->addChildrens( );
	root->findSolution( solutions );
	findTheBestSolution( );
}

/**
 * Prywatna metoda przeszukujaca drzewo i znajdujaca rozwiazania.
 */
void SearchTree::findTheBestSolution( )
{
	for ( auto node : solutions ) {
		if ( bestSolution != nullptr ) {
			if ( bestSolution->getMoves( ) > node->getMoves( ) ) {
				bestSolution = node;
			}
		} else {
			bestSolution = node;
		}
	}

}

/**
 * Pobranie liczby ruchow w najlepszym rozwiazaniu.
 */
unsigned int SearchTree::getMoves( ) const
{
	if ( bestSolution != nullptr ) {
		return bestSolution->getMoves( );
	}
	return 0;
}

/**
 * Pobranie stanu najlepszego rozwiazania.
 */
bool SearchTree::getState( ) const
{
	if ( bestSolution != nullptr ) {
		return bestSolution->getState( );
	}
}

/**
 * Wyswietlenie stanu pojemnikow.
 */
void SearchTree::showSolution( ) const
{
	if ( bestSolution != nullptr ) {
		bestSolution->showInfo( );
	} else {
		std::cout << "No solution found..." << std::endl;
	}
}