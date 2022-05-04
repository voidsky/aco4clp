#include "ant.h"

using namespace std;

Ant::Ant(Map& map, Container& cont)
{
	mMap = map;
	mCont = cont;
	// copy from map to local available nodes
	for (size_t n = 0; n < mMap.getNumberOfPackets(); n++)
		availableNodes.push_back(mMap.getPacket(n));
}


bool Ant::chooseFirst() 
{
	if (availableNodes.empty()) return false;
	int itemIndex = uniformRandom(availableNodes.size());
	Node * randomNode = &availableNodes.at(itemIndex);
	int randomOrientation = uniformRandom(randomNode->orientations);
	randomNode->orientation = randomOrientation;
	bool added = addToPath(*randomNode);
	if (added) availableNodes.erase( availableNodes.begin() + itemIndex );
	return added;
}

bool Ant::chooseInOrder() 
{
	if (availableNodes.empty()) return false;
	sort(mMap.mPackets.begin(), mMap.mPackets.end(),[](Node a, Node b) {
		return a.volume < b.volume;
	});	

	for (int index = 0; index < availableNodes.size(); index++) {
		Node * node = &availableNodes.at(index);
		int randomOrientation = uniformRandom(node->orientations);
		node->orientation = randomOrientation;
		//cout << node->volume << endl;
		addToPath(*node);
	}
	return true;
}

double Ant::SumLeftCriteriaWithPheromones(double alpha, double beta) 
{
	double sum = 0.0;
	int i = mPath.back().id;
	for (size_t n = 0; n < availableNodes.size(); n++)
	{
		int j = availableNodes.at(n).id;
		for (int o = 0; o < availableNodes.at(n).orientations; o++)
			sum += pow(mMap.getPhValue(i,j,o),alpha) * pow(availableNodes.at(n).volume,beta);	
			//sum += mMap.getPhValue(i,j,o) * availableNodes.at(n).volume;
			//sum += pow(mMap.getPhValue(i,j,o),alpha) * pow(availableNodes.at(n).volume,beta);					
			
	}
	return sum;
}

bool Ant::chooseNext(double alpha, double beta, bool shuffleOrder) 
{
	if (availableNodes.empty()) return false;			
	if (isFinished) return false;
			
	double sumLeftovers = SumLeftCriteriaWithPheromones(alpha, beta);

	double maxProbability = 0.0;
	int bestNextIndex;
	int bestOrientation = 0;
	bool candidateToAddFound = false;

	int i = mPath.back().id;

	/*if (shuffleOrder) {
		random_device rd;
    	mt19937 g(rd());
		shuffle(availableNodes.begin(), availableNodes.end(), g);
	}*/

	/*for (Node n : availableNodes) {
		if (n.orientations > 1) n.orientation = uniformRandom(n.orientations);
	}*/

	for (size_t n = 0; n < availableNodes.size(); n++)
	{
		Node * jnode = &availableNodes.at(n);
		int j = jnode->id;
		if (i==j) continue;
		
		double volumeOfj = jnode->volume;

		for (int o = 0; o < jnode->orientations; o++)
		{								
			double phij = mMap.getPhValue(i,j,o);
			double probability = ( (pow(phij,alpha) * pow(volumeOfj,beta))  / sumLeftovers);//* (double)(rand() / (RAND_MAX + 1.));
			//double q = randfrom(0,1);
			//double q0 = 0.0;	// probability to modify node, 1 100%, 0 0%
			//double psi = 1.2;	// value by which we modify calculated probalility to choose this node
			//if (q <= q0) probability *= psi;

			if (probability > maxProbability) {
				maxProbability = probability;
				bestNextIndex = n;
				bestOrientation = o;
				candidateToAddFound = true;
				}
		}
	}

	if (candidateToAddFound) 
	{
		isFinished = false;
		Node * bestNode = &availableNodes.at(bestNextIndex);
		bestNode->orientation = bestOrientation;
		bool added = addToPath(*bestNode);
		if (!added && bestNode->orientations > 1) 
		{
			bestNode->orientations--;					
		} else 
		{
			availableNodes.erase( availableNodes.begin() + bestNextIndex );
		}
		return true;
	} else
	{
				isFinished = true;
				return false;
	}
}	

bool Ant::chooseNextACS(double alpha, double beta, bool shuffleOrder) 
{
	if (availableNodes.empty()) return false;			
	if (isFinished) return false;
			
	double maxProbability = 0.0;
	int bestNextIndex;
	int bestOrientation = 0;
	bool candidateToAddFound = false;

	int i = mPath.back().id;

	if (shuffleOrder) {
		random_device rd;
    	mt19937 g(rd());
		shuffle(availableNodes.begin(), availableNodes.end(), g);
	}

	for (size_t n = 0; n < availableNodes.size(); n++)
	{
		Node * jnode = &availableNodes.at(n);
		int j = jnode->id;
		if (i==j) continue;
		
		double volumeOfj = jnode->volume;

		for (int o = 0; o < jnode->orientations; o++)
		{								
			double phij = mMap.getPhValue(i,j,o);
			double probability = phij * pow(volumeOfj,beta);
						
			if (probability > maxProbability) {
				maxProbability = probability;
				bestNextIndex = n;
				bestOrientation = o;
				candidateToAddFound = true;
				}
		}
	}

	if (candidateToAddFound) 
	{
		isFinished = false;
		Node * bestNode = &availableNodes.at(bestNextIndex);
		bestNode->orientation = bestOrientation;
		bool added = addToPath(*bestNode);
		if (!added && bestNode->orientations > 1) 
		{
			bestNode->orientations--;					
		} else 
		{
			availableNodes.erase( availableNodes.begin() + bestNextIndex );
		}
		return true;
	} else
	{
				isFinished = true;
				return false;
	}
}	

void Ant::updatePheromonePath(double sur) 
{
	if (mPath.size()<=1) return;
	
	//cout << "Updating map " << mMap.mPheromones << endl;

	#pragma omp parallel for 
	for (size_t n = 1; n < mPath.size(); n++)
	{
		int i = mPath.at(n-1).id;
		int j = mPath.at(n).id;
		int o = mPath.at(n).orientation;
		double oldPhValue = mMap.getPhValue(i,j,o);
		double newPh = (oldPhValue + sur /*/10*/);
		mMap.setPhValue(i,j,o,newPh);
	}
}	

void Ant::updatePheromonePathElitist(Ant * eliteAnt) 
{
	if (mPath.size()<=1) return;
	
	#pragma omp parallel for 
	for (size_t n = 1; n < mPath.size(); n++)
	{
		int i = mPath.at(n-1).id;
		int j = mPath.at(n).id;
		int o = mPath.at(n).orientation;
		double oldPhValue = mMap.getPhValue(i,j,o);
		double newPh;
		if (eliteAnt->HasIJ(i,j)) {
			double eliteSur = eliteAnt->getSur();
			double thisSur = getSur();
			newPh = (oldPhValue + thisSur) + eliteSur * 0.5;
		} else {
			newPh = (oldPhValue +  getSur());
		}
		mMap.setPhValue(i,j,o,newPh);
	}
}	

void Ant::updatePheromonePathACS(double rho) 
{
	if (mPath.size()<=1) return;
	
	#pragma omp parallel for 
	for (size_t n = 1; n < mPath.size(); n++)
	{
		int i = mPath.at(n-1).id;
		int j = mPath.at(n).id;
		int o = mPath.at(n).orientation;
		double oldPhValue = mMap.getPhValue(i,j,o);
		double sur = getSur();
		double newPh = oldPhValue + sur * 2;
		
		mMap.setPhValue(i,j,o,newPh);
	}
}	

void Ant::updateAfterOneStepForACS(double xi, double tau0) 
{
	if (mPath.size()<2) return;
	int i = mPath.at(mPath.size()-2).id;
	int j = mPath.at(mPath.size()-1).id;
	int o = mPath.at(mPath.size()-1).orientation;
	double oldPhValue = mMap.getPhValue(i,j,o);
	double newPh = oldPhValue - 0.01;
	mMap.setPhValue(i,j,o,newPh);
}


bool Ant::HasIJ(int ii, int jj) 
{
	for (size_t n = 1; n < mPath.size(); n++)
	{
		int i = mPath.at(n-1).id;
		int j = mPath.at(n).id;		
		if (ii==i && jj==j) return true;
	}	
	return false;
}


bool Ant::addToPath(Node node) 
{
	if (mCont.evaluateIndividual2(node, mPathVolume, mPathWeight))
	{
		mPath.push_back(node);
		return true;
	} else {
		return false;
	}
}

double Ant::getSur() 
{
	return mPathVolume / mCont.volume;
}


double Ant::getVolumePercentage() 
{
	return (mPathVolume * 100) / mCont.volume;
}

int Ant::getPathSteps() 
{
	return mPath.size();
}

void Ant::printPathSteps() 
{
	int prevTypeId = -1;
	int prevOrient = -1;
	int count = -1;

	cout << "Path: ";
	for (size_t n = 0; n < mPath.size(); n++)
	{
		if (mPath.at(n).typeId != prevTypeId || mPath.at(n).orientation != prevOrient) 
		{
			if (count > 0 ) cout << "-" << count << " ";
			cout << mPath.at(n).typeId << mPath.at(n).orientation;
			count = 1;
		} else 
		{
			count++;
		}
		prevTypeId = mPath.at(n).typeId;
		prevOrient = mPath.at(n).orientation;
	}
	cout << "-" << count << " ";
	cout << endl;
	
}