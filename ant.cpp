#include "ant.h"

using namespace std;

Ant::Ant(Map& map, Container& cont, ANTOBJECTIVE objective):
	mMap(map), mCont(cont)
{
	mObjective = objective;
	mMap = map;
	mCont = cont;
	ptrsToPackets = new vector<P_Cont*>();	
	for (size_t i = 0; i < mMap.size(); i++)
		ptrsToPackets->push_back(mMap.getPacketPtr(i));						
}

bool Ant::chooseFirst() 
		{
			if (ptrsToPackets->empty()) return false;
			int itemIndex = uniformRandom(ptrsToPackets->size());			
			addToPath(ptrsToPackets->at(itemIndex));
			ptrsToPackets->erase( ptrsToPackets->begin() + itemIndex );
			return true;
		}

double Ant::SumLeftCriteriaWithPheromones() 
{
			double sumVolumeTimesPh = 0.0;
			for (size_t n = 0; n < ptrsToPackets->size(); n++)
			{
				int j = ptrsToPackets->at(n)->id;
				switch (mObjective)
				{
				case OBJ_VOLUME:
					sumVolumeTimesPh += ptrsToPackets->at(n)->volume * mMap.getPhValue(j);
					break;					
				case OBJ_WEIGHT:
					sumVolumeTimesPh += ptrsToPackets->at(n)->weight * mMap.getPhValue(j);
					break;
				case OBJ_WEIGHTANDVOLUME:
					sumVolumeTimesPh += ptrsToPackets->at(n)->volume * mMap.getPhValue(j) + ptrsToPackets->at(n)->weight * mMap.getPhValue(j);
					break;
				}				
			}
			return sumVolumeTimesPh;
}
bool Ant::chooseNext() 
		{
			if (ptrsToPackets->empty()) return false;			
			
			double sumLeftovers = SumLeftCriteriaWithPheromones();

			double maxProbability = 0.0;
			int bestNextIndex;
			bool candidateToAddFound = false;

			for (size_t n = 0; n < ptrsToPackets->size(); n++)
			{
				int j = mMap.getPacketPtr(n)->id;
				double volumeOfj = mMap.getPacketPtr(n)->volume;
				double bottomAreaOfj = mMap.getPacketPtr(n)->bottomArea;
				double weiOfj = mMap.getPacketPtr(n)->weight;
				double phij = mMap.getPhValue(j);
				
				double probability;
				bool fitsInContainer;
				switch (mObjective)
				{
					case OBJ_VOLUME:
						probability = ((phij * volumeOfj)  / sumLeftovers) * (double)(rand() / (RAND_MAX + 1.));
						break;					
					case OBJ_WEIGHT:
						probability = (phij * weiOfj) / sumLeftovers;
						break;
					case OBJ_WEIGHTANDVOLUME:
						probability = (phij * weiOfj + phij * volumeOfj) / sumLeftovers;			
						break;
				}
				if (probability > maxProbability) {
					maxProbability = probability;
					bestNextIndex = n;
					candidateToAddFound = true;
				}
			}

			if (candidateToAddFound) 
			{
				if (addToPath(ptrsToPackets->at(bestNextIndex))) {
					ptrsToPackets->erase( ptrsToPackets->begin() + bestNextIndex );
					return true;
				} else {
					return false;
				}				
			} else
			{
				return false;
			}
		}	

void Ant::updatePheromonePath(double sur) 
{
	if (mPath.size()<=1) return;

	for (size_t n = 0; n < mPath.size(); n++)
	{
		int j = mPath.at(n).ptrToMapNode->id;		
		double newPh = (mMap.getPhValue(j) + sur);
		mMap.setPhValue(j,newPh);
	}	
}		

bool Ant::addToPath(P_Cont * packet) 
{
			// add it to ants path			
			Node n;
			n.ptrToMapNode = packet;
			n.typeId = n.ptrToMapNode->typeId;
			n.orientation = uniformRandom(n.ptrToMapNode->orientations);
			n.weight = n.ptrToMapNode->weight;
			n.count = 1;

			//mCont.AddNode(n);
			if (mCont.evaluateIndividual2(n, mPathVolume, mPathWeight))
			{
				mPath.push_back(n);
				//cout << mPathVolume << " ";
				return true;
			} else {
				//cout << mPathVolume << " ";				
				return false;
			}
}

double Ant::getSur() 
{
	double sur;
	switch (mObjective)
	{
		case OBJ_VOLUME:
			sur = mPathVolume / mCont.volume;
			break;
		case OBJ_WEIGHT:
		 	sur = mPathWeight / mCont.weight;
			break;
		case OBJ_WEIGHTANDVOLUME:
			sur = (mPathVolume + mPathWeight) / (mCont.volume + mCont.weight);
			break;
	}
	return sur;
}


double Ant::getVolumePercentage() {
	return (mPathVolume * 100) / mCont.volume;
}

double Ant::getWeightPercentage() {
	return (mPathWeight * 100) / mCont.weight;
}

int Ant::getPathSteps() {
	return mPath.size();
}