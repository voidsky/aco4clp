#include "ant.h"

using namespace std;

Ant::Ant(Map& map)
{
	mMap = map; 
	mCont = Container(map.mData);
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

bool Ant::chooseInOrder(int orientation) 
{
	if (availableNodes.empty()) return false;
	sort(mMap.mPackets.begin(), mMap.mPackets.end(),[](Node a, Node b) {
		return a.volume < b.volume;
	});	

	for (size_t index = 0; index < availableNodes.size(); index++) {
		Node * node = &availableNodes.at(index);
		int randomOrientation;
		if (orientation == -1 ) {
			randomOrientation= uniformRandom(node->orientations);
		} else {
			randomOrientation = orientation;
		}
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
	int oi = mPath.back().orientation;
	for (size_t n = 0; n < availableNodes.size(); n++)
	{
		int j = availableNodes.at(n).id;
		for (int oj = 0; oj < availableNodes.at(n).orientations; oj++)
			sum += pow(mMap.getPhValue(i,j,oi,oj),alpha) * pow(availableNodes.at(n).volume,beta);	
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
	int oi = mPath.back().orientation;

	if (shuffleOrder) {
		random_device rd;
    	mt19937 g(rd());
		shuffle(availableNodes.begin(), availableNodes.end(), g);
	}

	/*for (Node n : availableNodes) {
		if (n.orientations > 1) n.orientation = uniformRandom(n.orientations);
	}*/

	for (size_t n = 0; n < availableNodes.size(); n++)
	{
		Node * jnode = &availableNodes.at(n);
		int j = jnode->id;
		if (i==j) continue; // we dont move to same node
		
		double volumeOfj = jnode->volume;

		//TODO: need to changes this to process more than 2 orientations
		int oj;
		double phij;
		if (jnode->orientations > 1) {
			// node has two orientations, choose one with better pheromones
			double phijoj0 = mMap.getPhValue(i,jnode->id,oi,0);
			double phijoj1 = mMap.getPhValue(i,jnode->id,oi,1);		
			if (phijoj0 > phijoj1) {
				phij = phijoj0;
				oj = 0;
			} else if (phijoj0 < phijoj1) {
				phij = phijoj1;
				oj = 1;
			} else {
				// pheromones are equal, use default orientation
				phij = phijoj0;
				oj = jnode->orientation;
			}
		} else {
			// node has one orientation left, so we consider it for probability
			oj = jnode->orientation;
			phij = mMap.getPhValue(i,jnode->id,oi,oj);
		}
		
		double probability = ( (pow(phij,alpha) * pow(volumeOfj,beta))  / sumLeftovers);//* (double)(rand() / (RAND_MAX + 1.));

		if (probability > maxProbability) {
			maxProbability = probability;
			bestNextIndex = n;
			bestOrientation = oj;
			candidateToAddFound = true;
		}

	}

	if (candidateToAddFound) 
	{
		isFinished = false;
		Node * bestNode = &availableNodes.at(bestNextIndex);
		bestNode->orientation = bestOrientation;

		bool added = addToPath(*bestNode);
		if (added) {
			availableNodes.erase( availableNodes.begin() + bestNextIndex );
			return true;
		}
		// was not added, check orientations
		if (bestNode->orientations > 1) 
		{			
			// was not added, but more orientations left
			bestNode->orientations--;
			(bestNode->orientation == 0) ? bestNode->orientation = 1 : bestNode->orientation = 0;
			return true; 
		} else 
		{
			// was not added and has no more orientations left
			availableNodes.erase( availableNodes.begin() + bestNextIndex );
			return false;
		}
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
	int oi = mPath.back().orientation;

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

		for (int oj = 0; oj < jnode->orientations; oj++)
		{								
			double phij = mMap.getPhValue(i,j,oi, oj);
			double probability = phij * pow(volumeOfj,beta);
						
			if (probability > maxProbability) {
				maxProbability = probability;
				bestNextIndex = n;
				bestOrientation = oj;
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
		if (added) {
			availableNodes.erase( availableNodes.begin() + bestNextIndex );
			return true;
		}
		// was not added, check orientations
		if (bestNode->orientations > 1) 
		{			
			// was not added, but more orientations left
			bestNode->orientations--;
			(bestNode->orientation == 0) ? bestNode->orientation = 1 : bestNode->orientation = 0;
			return true; 
		} else 
		{
			// was not added and has no more orientations left
			availableNodes.erase( availableNodes.begin() + bestNextIndex );
			return false;
		}
	} else
	{
		isFinished = true;
		return false;
	}
}	

void Ant::updatePheromonePath(double sur) 
{
	if (mPath.size()<=1) return;

	#pragma omp parallel for 
	for (size_t n = 1; n < mPath.size(); n++)
	{
		int i = mPath.at(n-1).id;
		int oi = mPath.at(n-1).orientation;
		int j = mPath.at(n).id;		
		int oj = mPath.at(n).orientation;
		double oldPhValue = mMap.getPhValue(i,j,oi,oj);
		double newPh = (oldPhValue + sur);
		mMap.setPhValue(i,j,oi,oj,newPh);
	}
}	

void Ant::updatePheromonePathElitist(Ant * eliteAnt) 
{
	if (mPath.size()<=1) return;
	
	#pragma omp parallel for 
	for (size_t n = 1; n < mPath.size(); n++)
	{
		int i = mPath.at(n-1).id;
		int oi = mPath.at(n-1).orientation;
		int j = mPath.at(n).id;		
		int oj = mPath.at(n).orientation;
		double oldPhValue = mMap.getPhValue(i,j,oi,oj);
		double newPh;
		if (eliteAnt->HasIJ(i,j)) {
			double eliteSur = eliteAnt->getSur();
			double thisSur = getSur();
			newPh = oldPhValue + eliteSur * 1.5;
		} else {
			newPh = (oldPhValue +  getSur());
		}
		mMap.setPhValue(i,j,oi,oj,newPh);
	}
}	

void Ant::updatePheromonePathACS(Ant * eliteAnt, double rho) 
{
	if (mPath.size()<=1) return;
	
	#pragma omp parallel for 
	for (size_t n = 1; n < mPath.size(); n++)
	{
		int i = mPath.at(n-1).id;
		int oi = mPath.at(n-1).orientation;
		int j = mPath.at(n).id;		
		int oj = mPath.at(n).orientation;
		double oldPhValue = mMap.getPhValue(i,j,oi,oj);
		double newPh;
		if (eliteAnt->HasIJ(i,j)) {
			double eliteSur = eliteAnt->getSur();
			newPh = (1-rho)*oldPhValue + rho * eliteSur;
			mMap.setPhValue(i,j,oi,oj,newPh);
		} 		
	}
}	

void Ant::updateAfterOneStepForACS(double xi, double tau0) 
{
	if (mPath.size()<2) return;
	int i = mPath.at(mPath.size()-2).id;
	int oi = mPath.at(mPath.size()-2).orientation;
	int j = mPath.at(mPath.size()-1).id;
	int oj = mPath.at(mPath.size()-1).orientation;
	double oldPhValue = mMap.getPhValue(i,j,oi,oj);
	double newPh = (1-xi) * oldPhValue + xi * tau0;
	mMap.setPhValue(i,j,oi,oj,newPh);
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

double Ant::maxPheromone() 
{
	if (mPath.size()<=1) return 0;
	double max;
	for (size_t n = 1; n < mPath.size(); n++)
	{
		int i = mPath.at(n-1).id;
		int oi = mPath.at(n-1).orientation;
		int j = mPath.at(n).id;		
		int oj = mPath.at(n).orientation;
		double oldPhValue = mMap.getPhValue(i,j,oi,oj);
		if (n==1) {
			max = oldPhValue;
		} else {
			if (oldPhValue>max) max= oldPhValue;
		}
	}
	return max;
}

double Ant::minPheromone() 
{
	if (mPath.size()<=1) return 0;
	double min;
	for (size_t n = 1; n < mPath.size(); n++)
	{
		int i = mPath.at(n-1).id;
		int oi = mPath.at(n-1).orientation;
		int j = mPath.at(n).id;		
		int oj = mPath.at(n).orientation;
		double oldPhValue = mMap.getPhValue(i,j,oi,oj);
		if (n==1) {
			min = oldPhValue;
		} else {
			if (oldPhValue<min) min= oldPhValue;
		}
	}
	return min;
}

void Ant::printPathPheromones() 
{
	if (mPath.size()<=1) return;
	for (size_t n = 1; n < mPath.size(); n++)
	{
		int i = mPath.at(n-1).id;
		int oi = mPath.at(n-1).orientation;
		int j = mPath.at(n).id;		
		int oj = mPath.at(n).orientation;
		double oldPhValue = mMap.getPhValue(i,j,oi,oj);
		cout << oldPhValue << " ";
	}
}
