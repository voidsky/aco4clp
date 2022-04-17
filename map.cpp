#include "map.h"


Map::Map(ClpData& data):mData(data)
{
	mData = data;
	int id = 0;

    for (int type = 0; type < data.numPacketTypes; type++) 
	for (int packet = 0; packet < data.number[type]; packet++)
	{                
		Node n;
		n.id = id++;
		n.typeId = type;
		n.orientations = data.orient[type];										
		n.orientation = 0;
		n.volume = data.wid[type] * data.len[type] * data.hei[type];
		n.weight = data.wei[type];
		mPackets.push_back(n);
    }
}

void Map::initPheromones(double initialPhValue) 
{
    mPheromones = new double**[mPackets.size()];
    for (size_t i = 0; i < mPackets.size(); i++) 
	{
		mPheromones[i] = new double*[mPackets.size()];
		for (size_t j = 0; j < mPackets.size(); j++) 
		{
			mPheromones[i][j] = new double[mPackets.at(j).orientations];
			for (int o = 0; o < mPackets.at(j).orientations; o++)
			{	
				setPhValue(i,j,o,initialPhValue);
			}
		}
	}
}

void Map::evaporate() 
{
    for (size_t i = 0; i < mPackets.size(); i++) 
	{
		for (size_t j = 0; j < mPackets.size(); j++) 
		{
			for (int o = 0; o < mPackets.at(j).orientations; o++)
			{	
				double newPh = mPheromones[i][j][o] * (1-evaporationRate);
				setPhValue(i,j,o,newPh);				
			}
		}
	}
}

void Map::setPhValue(int i, int j, int o, double p) 
{ 
	if ((p>=mMinPh) && (p<=mMaxPh)) 
		mPheromones[i][j][o] = p; 
}

double Map::getPhValue(int i, int j, int o) { 
	return mPheromones[i][j][o]; 
};


void Map::printPheromones() 
{
	cout << "Print pheromones:" <<endl;
	int n = mPackets.size();
	for (int i = 0; i < n; i++)
	{	
		for (int j = 0; j < n; j++)
		{
			cout << "(";
			for (int o = 0; o < mPackets.at(j).orientations; o++)
				cout << getPhValue(i,j,o) << ",";
			cout << ") ";
		}
	}
	cout << endl;
}
