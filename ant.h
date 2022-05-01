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
		Ant(Map& map, Container& cont);	
		bool chooseFirst();
		bool chooseNext(double alpha, double beta, bool shuffleOrder);
		void updatePheromonePath(double sur);
		int getPathSteps();
		void printPathSteps();
		double getSur();
        
		double getVolumePercentage();
		double getWeightPercentage();

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