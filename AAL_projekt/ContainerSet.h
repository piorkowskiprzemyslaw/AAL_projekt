#ifndef CONTAINERSET_H
#define CONTAINERSET_H

#include <iostream>
#include <vector>
#include <fstream>
#include "Container.h"
#include "Color.h"
#include "cmath"

enum Direction {
	left = 0,
	right = 1
};

/**
 * Zbior pojemnikow z wazna relacja sasiedztwa konkretnrych pojemnikow.
 */
class ContainerSet
{
private:
	// Tablica kontenerow.
	std::vector<Container> containers;
	//suma pojemnosci wszystkich pojemnikow.
	size_t capacitySum;

public:
	//Klasa iteratora do przechodzenia po zbiorze pudelek
	class const_iterator
	{
	protected:
		std::vector<Container> & container;
		std::vector<Container>::iterator iter;
	public:
		const_iterator( const const_iterator & a ) : container( a.container ), iter( a.iter ) { }
		const_iterator( std::vector<Container> & cont, const size_t pos ) : container( cont )
		{
			iter = cont.begin( ) + pos;
			container = cont;
		}

		inline const Container * operator->( ) const { return iter.operator ->( ); }
		inline const Container& operator*( )  const { return iter.operator *( ); }

		// przejscie w prawo
		const_iterator& operator++( )
		{
			++iter;
			if ( iter == container.end( ) )
				iter = container.begin( );
			return *this;
		}

		const_iterator operator++( int )
		{
			const_iterator backup( *this );
			this->operator++( );
			return backup;
		}

		// przejscie w lewo
		const_iterator& operator--( )
		{
			if ( iter == container.begin( ) ) {
				iter = container.end( );
				--iter;
			} else {
				--iter;
			}
			return *this;

		}

		const_iterator operator--( int )
		{
			const_iterator backup( *this );
			this->operator--( );
			return backup;
		}
	};
	
	//Klasa zwyklego iteratora.
	class iterator: public const_iterator
	{
	public:
		iterator( const const_iterator & a ) : const_iterator( a ) { }
		iterator( std::vector<Container> & cont, const size_t pos ) : const_iterator( cont, pos ) { }

		inline Container& operator*( ) const { return this->iter.operator *( ); }
		inline Container* operator->( ) const { return this->iter.operator ->( ); }
		inline bool operator==( const iterator & another ) const { return this->iter == another.iter; }
		inline bool operator!=( const iterator & another ) const { return this->iter != another.iter; }

		iterator& operator=( const iterator & another )
		{
			if ( *this == another )
				return *this;
			this->iter = another.iter;
			this->container = another.container;
			return *this;
		}

		iterator& operator++( )
		{
			++( *( const_iterator* )this );
			return ( *this );
		}

		iterator operator++( int )
		{
			iterator temp = *this;
			++*this;
			return temp;
		}

		iterator& operator--( )
		{
			--( *( const_iterator* )this );
			return ( *this );
		}

		iterator operator--( int )
		{
			iterator temp = *this;
			--*this;
			return temp;
		}

	};

	ContainerSet( const std::vector<unsigned int> & containersCapacity, const std::vector<std::vector<unsigned int> > & includingList, const unsigned int colorsNumber );
	~ContainerSet( );
	explicit ContainerSet( const ContainerSet& another );
	void showInfo() const;
	void showToFile( const char * fileName ) const;
	unsigned int getColorsNumber() const;
	unsigned int colorMultiplicity(const Color & color) const;
	size_t size() const;
	size_t getCapacitySum() const;
	iterator getMaxiumWithColor(const Color & color);
	iterator begin();
	unsigned int swapBlocksWithFreeSpace(const iterator & freeSpace, const iterator & firstBlockLocation, const Color & firstColor, const iterator & secondBlockLocation, const Color & secondColor);
	unsigned int swapBlockFreeSpace( const iterator & freeSpace, const iterator & blockLocation, const Color & color );
	bool checkState( ) const;
	bool checkForNeighborhood( const iterator & first, const iterator & second ) const;

private:
	unsigned int getDistance(Direction dir, const iterator & first, const iterator & second) const;
	unsigned int shiftBlocks(std::vector<Color *> & shfitVector, const iterator & freeSpace, unsigned int distance, Direction dir);

};


#endif
