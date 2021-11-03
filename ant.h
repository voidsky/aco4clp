#ifndef ANT_H
#define ANT_H

#include "CLP.h"
#include "struct.h"
#include "map.h"

class Ant {
	public:
		Ant(CLP clp, Map& map, double maxContainerVolume, double maxContainerWeight, ANTOBJECTIVE objective);	
		bool chooseFirst();
		bool chooseNext();
		void updatePheromonePath(double sur);
		double getSur();
        double SumLeftCriteriaWithPheromones();

		double mMaxContainerVolume;
		double mMaxContainerWeight;
		double mPathVolume = 0.0;
		double mPathWeight = 0.0;
		double mSur = 0.0;
		CLP mClp;
		std::vector<Node> *mPath;
	private:
		ANTOBJECTIVE mObjective;
		bool addToPath(P_Cont * packet);
		std::vector<P_Cont*> *ptrsToPackets;
			
		double * mPheromones;	
		Map mMap;
		
};

#endif 