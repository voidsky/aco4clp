#include "map.h"


Map::Map(ClpData& data):mData(data)
{
	mData = data;
	mPackets = new vector<P_Cont>();

        int id = 0;

        for (int type = 0; type < data.numPacketTypes; type++) 
                for (int packet = 0; packet < data.number[type]; packet++)
                {                
                                        P_Cont n;
                                        n.id = id++;
                                        n.typeId = type;
                                        n.orientations = data.orient[type];
                                        n.bottomArea = data.wid[type] * data.len[type];
                                        n.volume = data.wid[type] * data.len[type] * data.hei[type];
                                        n.weight = data.wei[type];
                                        mPackets->push_back(n);
                }

}

void Map::initPheromones(double initialPhValue) 
{
    mPheromones = new double[mPackets->size()];
    for (size_t j = 0; j < mPackets->size(); j++) setPhValue(j,initialPhValue);

}

void Map::evaporate() 
{
	int n = mPackets->size();
	for (int j = 0; j < n; j++)
	{
		double newPh = mPheromones[j] * (1-evaporationRate);
		setPhValue(j,newPh);
	}
}

void Map::setPhValue(int j, double p) 
{ 
	if ((p>=mMinPh) && (p<=mMaxPh)) 
		mPheromones[j] = p; 
}

void Map::printPheromones() 
{
	int n = mPackets->size();
	for (int j = 0; j < n; j++)
	{
		cout << mPheromones[j] << " ";
	}
	cout << endl;
}
