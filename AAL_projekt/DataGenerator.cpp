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
	//loteria zaokraglenia liczby.
	float lottery = 0;
	//tablica z przedzialami do rodzielania kolorow wg rozkladu normalnego.
	float* colorsInterval = new float[colorsNumber + 1];
	//rozk³ad równomierny do pojemnoœci pojemników
	std::uniform_int_distribution<int> intDistribution(static_cast<int>(minCapacity), static_cast<int>(maxCapacity));
	//rozk³ad równomierny do kolorów pojemników
	std::uniform_real_distribution<double> realDistribution(0.0, 1.0);
	//Aktualnie wylosowany kolor
	float color;
	//Mapa z mo¿liwoœci¹ wstawiania konkretnych kolorów.
	std::map<unsigned int, unsigned int > colorsMap;

	for (size_t i = 0; i < static_cast<size_t>(colorsNumber); i++)
	{
		colorsMap.insert({ i, numberOfContainers });
	}
	
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
	{//jesli generowane dane musza byc takie jak w opisie zadania tzn zadnego koloru nie moze byc wiecej niz n to musi zostac to co odkomentowane.
	 //w innym przypadku ³adniejsze jest to co jest zakomentowane i tego nalezy sie trzymac.

		//degree = containerscapacity[i] * fillingdegree;
		//for (size_t k = 0; k < static_cast<size_t>(degree) ; k++)
		//{
		//	color = realdistribution(engine);
		//	for (size_t m = 0; m < static_cast<size_t>(colorsnumber + 1); m++)
		//	{
		//		if (color < colorsinterval[m])
		//		{
		//			includinglist[i].push_back(m-1);
		//			break;
		//		}
		//	}
		//}
		bool flag = false;
		int counter = 0;

		degree = containersCapacity[i] * fillingDegree;

		//loteria dotyczaca zapelniania kube³ka.
		lottery = realDistribution(engine);
		if (lottery <= 0.33)
		{
			degree--;
		}
		else if (lottery <= 0.66)
		{
			degree++;
		}

		while (true)
		{
			if (counter >= static_cast<int>(degree))
			{
				counter = 0;
				break;
			}

			color = realDistribution(engine);

			for (size_t m = 0; m < static_cast<size_t>(colorsNumber + 1); m++)
			{
				if (color < colorsInterval[m])
				{
					//sprawdzenie czy mozna jeszcze wstawiac
					if (colorsMap[m - 1])
					{
						includingList[i].push_back(m - 1);
						colorsMap[m - 1]--;
						flag = true;
					} else {
						flag = false;
					}

					break;
				}
			}

			if (flag)
			{
				counter++;
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