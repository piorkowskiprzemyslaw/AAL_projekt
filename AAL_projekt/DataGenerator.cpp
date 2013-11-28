#include "DataGenerator.h"

/*
 * Konstruktor pobieraj¹cy wszystkie potrzebna do wygenerowania dane.
 */
DataGenerator::DataGenerator(const unsigned int numberOfContainers, const unsigned int minCapacity, const unsigned int maxCapacity, const float fillingDegree, const unsigned int colorsNumber)
: numberOfContainers(numberOfContainers), minCapacity(minCapacity), maxCapacity(maxCapacity), fillingDegree(fillingDegree), colorsNumber(colorsNumber)
{
	if (fillingDegree >= 1)
	{
		std::cout << "fillingDegree cannot be more than 1!" << std::endl;
		exit(1);
	}
	engine.seed(static_cast<unsigned int>(time(NULL)));
}

/*
 * Destruktor...
 */
DataGenerator::~DataGenerator()
{
}

/*
 * metoda generujaca dane.
 */
void DataGenerator::generateData()
{
	unsigned int generate = 0;
	//wskaznik zapelnienia. Dla kazdego pojemnika bedzie obliczany oddzielnie
	float degree = 0;
	//tablica z przedzialami do rodzielania kolorow wg rozkladu normalnego.
	float* colorsInterval = new float[colorsNumber + 1];
	//rozk³ad równomierny do pojemnoœci pojemników
	std::uniform_int_distribution<int> intDistribution(static_cast<int>(minCapacity), static_cast<int>(maxCapacity));
	//rozk³ad równomierny do kolorów pojemników
	std::uniform_real_distribution<double> realDistribution(0.0, 1.0);
	//Aktualnie wylosowany kolor
	float color;
	
	//Inicjalizacja przedzia³ów dla rozk³adu normalnego
	colorsInterval[colorsNumber] = 1.0;
	for (size_t i = 0; i < static_cast<size_t>(colorsNumber); i++)
	{
		colorsInterval[i] = static_cast<float>(i) / static_cast<float>(colorsNumber);
	}

	//generowanie pojemnosci dla pojemnikow
	for (size_t i = 0; i < numberOfContainers; i++)
	{
		containersCapacity.push_back(intDistribution(engine));
	}

	//zapelnianie includingLista pustymi vectorami...
	for (size_t i = 0; i < numberOfContainers; i++)
	{
		includingList.push_back(std::vector<unsigned int>());
	}

	//zapelnianie wszystkich pojemników.
	for (size_t i = 0; i < static_cast<size_t>(includingList.size()); i++)
	{
		degree = containersCapacity[i] * fillingDegree;
		for (size_t k = 0; k < static_cast<size_t>(degree) ; k++)
		{
			color = realDistribution(engine);
			for (size_t m = 0; m < static_cast<size_t>(colorsNumber + 1); m++)
			{
				if (color < colorsInterval[m])
				{
					includingList[i].push_back(m-1);
					break;
				}
			}
		}
	}
}

/*
 * Pobranie pojemnoœci pojemników.
 */
std::vector<unsigned int> & DataGenerator::getConatinersCapacity()
{
	return containersCapacity;
}

/*
 * Pobranie listy zawierania.
 */
std::vector<std::vector<unsigned int> > & DataGenerator::getIncludingList()
{
	return includingList;
}

/*
 * Pobranie iloœci kolorów.
 */
unsigned int DataGenerator::getColorsNumber()
{
	return colorsNumber;
}

/*
 * Wyswietlenie wygenerowanych informacji
 */
void DataGenerator::showGeneratedInfo()
{
	for (size_t i = 0; i < includingList.size(); i++)
	{
		std::cout << "Container number : " << i << " Capacity : " << containersCapacity[i] << " colors inside :" << std::endl;
		for (size_t j = 0; j < includingList[i].size(); j++)
		{
			std::cout << includingList[i][j] << " ";
		}
		std::cout << std::endl;
	}
}