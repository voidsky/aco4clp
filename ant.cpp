#include "ant.h"
using namespace std;

Ant::Ant(CLP clp, Map& map, double maxContainerVolume, double maxContainerWeight, ANTOBJECTIVE objective) 
{
	mClp = clp;
	mObjective = objective;
	mMap = map;
	mPheromones = map.pheromones;		
	mMaxContainerVolume = maxContainerVolume;
	mMaxContainerWeight = maxContainerWeight;
	mPath = new vector<Node>();	
	ptrsToPackets = new vector<P_Cont*>();	
	for (size_t i = 0; i < mMap.packets->size(); i++)
		ptrsToPackets->push_back(&mMap.packets->at(i));						
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
					sumVolumeTimesPh += ptrsToPackets->at(n)->volume + mPheromones[j];
					break;					
				case OBJ_WEIGHT:
					sumVolumeTimesPh += ptrsToPackets->at(n)->weight * mPheromones[j];
					break;
				case OBJ_WEIGHTANDVOLUME:
					sumVolumeTimesPh += ptrsToPackets->at(n)->volume * mPheromones[j] + ptrsToPackets->at(n)->weight * mPheromones[j];
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
				int j = ptrsToPackets->at(n)->id;
				double volumeOfj = ptrsToPackets->at(n)->volume;
				double bottomAreaOfj = ptrsToPackets->at(n)->bottomArea;
				double weiOfj = ptrsToPackets->at(n)->weight;
				double phij = mPheromones[j];
				
				double probability;
				bool fitsInContainer;
				switch (mObjective)
				{
					case OBJ_VOLUME:
						probability = ((phij + volumeOfj)  / sumLeftovers)  * (double)(rand() / (RAND_MAX + 1.));
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
	if (mPath->size()<=1) 
	{
		//cout << " Ant has one or less nodes, nothing to update." << endl;
		return;
	}
	mSur = sur;

	for (size_t n = 0; n < mPath->size(); n++)
	{
		int j = mPath->at(n).ptrToMapNode->id;		
		double newPh = mPheromones[j] = mSur;
		mMap.setPhValue(j,newPh);
	}	
}		

bool Ant::addToPath(P_Cont * packet) 
{
			// add it to ants path			
			Node n;
			n.ptrToMapNode = packet;
			n.typeId = n.ptrToMapNode->typeId;
			n.orientation = 0;			
			n.count = 1;
			

			/*double vol = 0;
			for (size_t i = 0; i < mPath->size(); i++)
			{
				vol += mPath->at(i).ptrToMapNode->volume;
			}
			cout << vol << " ";		*/

			mClp.AddNode(n);
			mClp.evaluateIndividual2(mPathVolume, mPathWeight);			
			if (mClp.AllPacked()) {
				mPath->push_back(n);
				//cout << mPathVolume << " ";
				return true;
			} else {
				mClp.RemoveLast(mPathVolume, mPathWeight);
				//cout << mPathVolume << " ";				
				return false;
			}
			//double *o1 = new double();
			//double *o2 = new double();
			//mClp.evaluateIndividual(mPath, o1, o2);
			//mPathVolume = *o1;
			//mPathWeight = *o2;
			//mPathVolume += n.ptrToMapNode->volume;
			//mPathWeight += n.ptrToMapNode->weight;
			
			//return true;
}

double Ant::getSur() 
{
	double sur;
	switch (mObjective)
	{
		case OBJ_VOLUME:
			sur = mPathVolume / mMaxContainerVolume;
			break;
		case OBJ_WEIGHT:
		 	sur = mPathWeight / mMaxContainerWeight;
			break;
		case OBJ_WEIGHTANDVOLUME:
			sur = (mPathVolume + mPathWeight) / (mMaxContainerVolume + mMaxContainerWeight);
			break;
	}
	return sur;
}
