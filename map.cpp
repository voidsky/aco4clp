#include "map.h"

Map::Map() 
{
	packets = new vector<P_Cont>();
}

void Map::initPheromoneMap(double initialPhValue) 
{
    pheromones = new double[packets->size()];
    for (size_t j = 0; j < packets->size(); j++) setPhValue(j,initialPhValue);

}

void Map::evaporate() 
{
	int n = packets->size();
	for (int j = 0; j < n; j++)
	{
		double newPh = pheromones[j] * (1-evaporationRate);
		setPhValue(j,newPh);
	}
}

void Map::setPhValue(int j, double p) 
{ 
	if ((p>=mMinPh) && (p<=mMaxPh)) 
		pheromones[j] = p; 
}

void Map::printPheromones() 
{
	int n = packets->size();
	for (int j = 0; j < n; j++)
	{
		cout << pheromones[j] << " ";
	}
	cout << endl;
}
