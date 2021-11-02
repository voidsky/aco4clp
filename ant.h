#ifndef ANT_H
#define ANT_H

#include "CLP.h"
#include "struct.h"

class Map {
	public:
		Map();
		void initPheromoneMap(double initialPhValue);
		void setEvaporationRate(double er) { evaporationRate = er; };		
		void evaporate();		
		void printPheromones();		
		std::vector<P_Cont> *packets;
		double ** pheromones;
	private:
		double evaporationRate;
};

class Ant {
	public:
		Ant(CLP * clp, Map* map, double maxContainerVolume);	
		bool chooseFirst();
		bool chooseNext();
		void updatePheromonePath(double sur);
		double getPathVolume();
        std::vector<Node> *getPath();
		void setSur(double s) { mSur = s; };
		double getSur(void) { return mSur; };
	private:
		CLP * mClp;
		bool addToPath(P_Cont * packet);
		std::vector<P_Cont*> *ptrsToPackets;
		std::vector<Node> *mPath;	
		double ** mPheromones;	
		Map* mMap;
		double mMaxContainerVolume;
		double mPathVolume = 0.0;
		double mSur = 0.0;
};

#endif 