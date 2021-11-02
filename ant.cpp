#include "ant.h"
using namespace std;


Map::Map() 
{
	packets = new vector<P_Cont>();
}
void Map::initPheromoneMap(double initialPhValue) 
{
    pheromones = new double*[packets->size()];
    for (size_t i = 0; i < packets->size(); i++)        
        {
                pheromones[i] = new double[packets->size()];
                for (size_t j = 0; j < packets->size(); j++) pheromones[i][j] = initialPhValue;                                        
        }	

}

void Map::evaporate() 
{
	int n = packets->size();
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			pheromones[i][j] *= (1-evaporationRate);
		}
		
	}	
}

void Map::printPheromones() 
{
	int n = packets->size();
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << pheromones[i][j] << " ";
		}
		cout << endl;
	}	
	cout << endl;
}

void Ant::updatePheromonePath(double sur) 
{
	if (mPath->size()<=1) 
	{
		//cout << " Ant has one or less nodes, nothing to update." << endl;
		return;
	}

	for (size_t n = 1; n < mPath->size(); n++)
	{
		int i = mPath->at(n-1).ptrToMapNode->id;		
		int j = mPath->at(n).ptrToMapNode->id;		
		mPheromones[i][j] += sur;
	}	
}


Ant::Ant(CLP * clp, Map* map, double maxContainerVolume) 
{
	mMap = map;
	mMaxContainerVolume = maxContainerVolume;
	mClp = clp;
	mPath = new vector<Node>();
	mPheromones = mMap->pheromones;
	ptrsToPackets = new vector<P_Cont*>();	
	for (size_t i = 0; i < mMap->packets->size(); i++)
		ptrsToPackets->push_back(&mMap->packets->at(i));						
}

bool Ant::chooseFirst() 
		{
			if (ptrsToPackets->empty()) return false;
			int itemIndex = uniformRandom(ptrsToPackets->size());			
			//cout << "Ant starting from id " << ptrsToPackets->at(itemIndex)->id << endl;
			// add to path and remove from ants known path so it wont be chosen next time
			addToPath(ptrsToPackets->at(itemIndex));
			ptrsToPackets->erase( ptrsToPackets->begin() + itemIndex );
			return true;
		}

bool Ant::chooseNext() 
		{
			if (ptrsToPackets->empty()) return false;			
			int i = mPath->back().ptrToMapNode->id;		// i is current node id
			
			double sumVolumeTimesPh = 0.0;
			for (size_t n = 0; n < ptrsToPackets->size(); n++)
			{
				int j = ptrsToPackets->at(n)->id;
				sumVolumeTimesPh += ptrsToPackets->at(n)->volume * mPheromones[i][j];
			}

			// itterate our local map copy
			double maxProbability = 0.0;
			int bestNextIndex;
			bool candidateToAddFound = false;
			for (size_t n = 0; n < ptrsToPackets->size(); n++)
			{
				int j = ptrsToPackets->at(n)->id;
				//double bottomAreaOfj =  ptrsToPackets->at(n)->bottomArea;
				double volumeOfj = ptrsToPackets->at(n)->volume;
				// get pheromone value of possible next step
				double phij = mPheromones[i][j];
				double probability = (phij * volumeOfj) / sumVolumeTimesPh;
				bool fitsInContainer = mPathVolume + volumeOfj <= mMaxContainerVolume;
				if (fitsInContainer && probability > maxProbability) {
					maxProbability = probability;
					bestNextIndex = n;
					candidateToAddFound = true;
				}
			}

			if (candidateToAddFound) 
			{
				addToPath(ptrsToPackets->at(bestNextIndex));
				ptrsToPackets->erase( ptrsToPackets->begin() + bestNextIndex );
				return true;
			} else
			{
				return false;
			}
		}	

vector<Node> * Ant::getPath() 
		{
			return mPath;
		}

bool Ant::addToPath(P_Cont * packet) 
{
			// add it to ants path			
			Node n;
			n.ptrToMapNode = packet;
			n.typeId = n.ptrToMapNode->typeId;
			n.orientation = 0;			
			n.count = 1;
			mPath->push_back(n);
			mPathVolume += n.ptrToMapNode->volume;
			return true;
}

double Ant::getPathVolume() 
{
	return mPathVolume;
}

