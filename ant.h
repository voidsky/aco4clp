#ifndef ANT_H
#define ANT_H

#include "struct.h"
#include "map.h"
#include "container.h"

class Ant {
	public:
		Ant(Map& map, Container& cont, ANTOBJECTIVE objective);	
		bool chooseFirst();
		bool chooseNext();
		void updatePheromonePath(double sur);
		int getPathSteps();
		double getSur();
        double SumLeftCriteriaWithPheromones();

		double getVolumePercentage();
		double getWeightPercentage();

	private:		
		ANTOBJECTIVE mObjective;
		bool addToPath(P_Cont * packet);
		std::vector<P_Cont*> *ptrsToPackets;
			
		Map mMap;
		Container mCont;

		double mPathVolume = 0.0;
		double mPathWeight = 0.0;
		
		std::vector<Node> mPath;		
};

#endif 