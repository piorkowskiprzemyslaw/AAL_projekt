#include "Node.h"

unsigned int Node::minMoves;

/**
 * Konstruktor domyslny.
 */
Node::Node( ) : state( false ), moves(0)
{
	this->containerSet = nullptr;
	this->parent = nullptr;
	Node::minMoves = UINT_MAX;
}

/**
 * Konstrutkor parametryczny. Praktycznie tylko dla roota - ustawia parent na null.
 */
Node::Node( const ContainerSet & containerSet ) : moves(0) 
{
	this->containerSet = new ContainerSet( containerSet );
	this->parent = nullptr;
	this->state = this->containerSet->checkState( );
	Node::minMoves = UINT_MAX;
	if ( this->state == true ) {
		if ( Node::minMoves > this->moves ) {
			Node::minMoves = this->moves;
		}
	}
}

/**
 * Konstrutkor parametryczny. Dla dzieci w drzewie.
 */
Node::Node( ContainerSet * containerSet, Node * parent, const unsigned int moves ) : moves(moves)
{
	this->containerSet = containerSet;
	this->parent = parent;
	this->state = this->containerSet->checkState( );
	if ( this->state == true ) {
		if ( Node::minMoves > this->moves ) {
			Node::minMoves = this->moves;
		}
	}
}

/**
 * Destruktor.
 */
Node::~Node( )
{
	delete( containerSet );
	for ( auto node : children ) {
		node->~Node( );
	}
}

/**
 * Automatyczne dodanie wszystkich mozliwych dzieci ( wykonanie wszystkich mozliwych ruchow ).
 */
void Node::addChildrens( )
{
	ContainerSet * csLeft = nullptr;
	ContainerSet * csRight = nullptr;
	ContainerSet::iterator container(containerSet->begin( ));
	std::list<Color *> * listOfColors;

	if ( state == true ) {
		return;
	}

	if ( moves + 1 > Node::minMoves ) {
		return;
	}

	do {

		listOfColors = container->getAllDifferentColors( );

		for ( std::list<Color *>::iterator color = listOfColors->begin( ); color != listOfColors->end( ); ++color ) {

			csLeft = new ContainerSet( *containerSet );
			ContainerSet::iterator csIteratorLeft( csLeft->begin( ), container->getIndex( ) );
			ContainerSet::iterator csLeftNeighbor( csIteratorLeft );
			--csLeftNeighbor;

			if ( csIteratorLeft->moveBlock( **color, *csLeftNeighbor ) ) {
				//powiodlo sie przeniesienie klocka kolor do lewego sasiada.

				if ( findACopy( csLeft ) == false )
					children.push_back( new Node( csLeft, this, moves + 1 ) );
				else
					delete( csLeft );
			
			} else {
				delete( csLeft );
			}

			csRight = new ContainerSet( *containerSet );
			ContainerSet::iterator csIteratorRight( csRight->begin( ), container->getIndex( ) );
			ContainerSet::iterator csRightNeighbor( csIteratorRight );
			++csRightNeighbor;

			if ( csIteratorRight->moveBlock( **color, *csRightNeighbor ) ) {
				//powiodlo sie przeniesienie klocka kolor do prawego sasaida.

				if ( findACopy( csRight ) == true )
					children.push_back( new Node( csRight, this, moves + 1 ) );
				else
					delete( csRight );
			
			} else {
				delete( csRight );
			}

			delete ( *color );
		}


		delete( listOfColors );
		++container;
	} while ( container != containerSet->begin( ) );


	for ( auto node : children ) {
		node->addChildrens( );
	}
}

/**
 * szukanie kopii syttuacji wyzej w drzewie.
 */
bool Node::findACopy( const ContainerSet * containerSet )
{
	Node * root = this;

	/*
	while ( tmp != nullptr ) {
		if ( *(tmp->containerSet) == *containerSet )
			return false;
		for ( auto node : tmp->children ) {
			if ( *(node->containerSet) == *containerSet ) {
				return false;
			}
		}
		tmp = tmp->parent;
	}*/

	while ( root->parent != nullptr ) {
		root = root->parent;
	}

	return root->check( containerSet, moves + 1 );
}

/**
 * Rekurencyjne sprawdzenie wszystkich elelemntow w drzewie.
 */
bool Node::check( const ContainerSet * containerSet, unsigned int heigh ) const
{
	if ( *(this->containerSet) == *containerSet ) {
		return true;
	}

	if ( heigh == 0 ) {
		return false;
	}

	for ( auto node : children ) {
		if ( node->check( containerSet, heigh - 1 ) == false ) {
			return true;
		}
	}

	return true;
}

/**
 * Znalezienie wszsytkich rozwiazan uzyskanych w drzewie.
 */
void Node::findSolution( std::list<Node *> & solutions )
{
	if ( state == true ) {
		solutions.push_back( this );
		return;
	}

	for ( auto node : children ) {
		node->findSolution( solutions );
	}

}

/**
 * Pokazanie informacji o znalezionym rozwiazaniu.
 */
void Node::showInfo( ) const
{
	containerSet->showInfo( );
}

/**
 * Pobranie ilosci ruchow w aktualnym node.
 */
unsigned int Node::getMoves( ) const
{
	return moves;
}

/**
 * Pobranie stanu zbioru pojemnikow.
 */
bool Node::getState( ) const
{
	return state;
}