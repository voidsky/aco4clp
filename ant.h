#ifndef ANT_H
#define ANT_H

#include "struct.h"
#include "map.h"
#include "container.h"
#include <cmath>

#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
using namespace std ; 

class Ant {
	public:
		Ant() {};
		Ant(Map& map);
		bool chooseFirst();
		bool chooseInOrder(int orientation);
		bool chooseNext(double alpha, double beta, bool shuffleOrder);
		bool chooseNextACS(double alpha, double beta, bool shuffleOrder);
		void updatePheromonePath(double sur);		
		void updatePheromonePathElitist(Ant * eliteAnt);		
		void updatePheromonePathACS(Ant * eliteAnt, double rho);
		void updateAfterOneStepForACS(double xi, double tau0);

		int getPathSteps();
		void printPathSteps();
		double getSur();		
        bool HasIJ(int ii, int jj);

		double getVolumePercentage();
		double getWeightPercentage();
		
		double maxPheromone();
		double minPheromone();
		void printPathPheromones();

	private:		
		double SumLeftCriteriaWithPheromones(double alpha, double beta);
		bool addToPath(Node node);
		bool isFinished = false;
		std::vector<Node> availableNodes;
			
		Map mMap;
		Container mCont;

		double mPathVolume = 0.0;
		double mPathWeight = 0.0;
		
		std::vector<Node> mPath;		

};

#endif 