#include "SearchTree.h"

/**
 * Domyslny konstrutkor
 */
SearchTree::SearchTree()
{
	root = nullptr;
	bestSolution = nullptr;
	containerSet = nullptr;
}

/**
 * Konstrutkor parametryczny.
 */
SearchTree::SearchTree( const ContainerSet & containerSet )
{
	root = new Node( containerSet );
	bestSolution = nullptr;
	this->containerSet = new ContainerSet( containerSet );
	creationQueue.push( root );
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
	std::cout << "Checking preconditions ... ";

	//sprawdzanie warunkow poczatkowych umozliwiajacych rozwiazanie zadania.
	if ( !checkPreconditions( ) ) {
		std::cout << "[FALSE]" << std::endl;
		std::cout << "End of solving problem" << std::endl;
		return;
	}
	std::cout << "[OK]" << std::endl;
	
	createTree( );
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

/**
 * Prywatna metoda sprawdzajaca spelnienie warunkow poczatkowych.
 */
bool SearchTree::checkPreconditions( ) const
{
	size_t* colorMultiplicity = new size_t[this->containerSet->getColorsNumber( )];
	unsigned int allColorSum = 0;

	//liczba kolorow musi byc mniejsza badz rowna liczbe dostepnych pojemnikow.
	if ( Color::getNumberOfAllColors( ) > this->containerSet->size( ) ) {
		return false;
	}

	//lacznie we wszystkich pojemnikach jest nie wiecej niz n klockow danego koloru.
	//przy okazji wykonuje zliczanie ka¿dego koloru - potrzebne do dalszej czesci algorytmu,
	for ( size_t i = 0; i < containerSet->getColorsNumber( ); i++ ) {
		colorMultiplicity[i] = containerSet->colorMultiplicity( i );
		allColorSum += colorMultiplicity[i];
		if ( colorMultiplicity[i] > containerSet->size( ) ) {
			return false;
		}

	}

	//zapewnienie aby wszystkie klocki miescily sie w pojemnikach, oraz zeby dalo
	//sie wykonac ruch.
	if ( allColorSum >= containerSet->getCapacitySum( ) ) {
		return false;
	}

	delete( colorMultiplicity );

	return true;
}

/**
 * Metoda odpowiedzialna za stworzenie drzewa przeszukiwan.
 */
void SearchTree::createTree( )
{
	Node * tmp;
	std::list<Node *> * creationList;
	while ( !creationQueue.empty( ) ) {
		tmp = creationQueue.front( );
		creationQueue.pop( );
		creationList = tmp->addChildrens( );

		for ( auto it = creationList->begin( ); it != creationList->end( ); ++it ) {
			creationQueue.push( *it );
		}

		std::cout << "CreationQueue size :" << creationQueue.size( ) << " Dodano : " << creationList->size() << " Aktualny poziom : " << tmp->getMoves() << std::endl;
		delete( creationList );
	}
}