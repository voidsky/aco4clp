#ifndef MAP_H
#define MAP_H

#include "CLP.h"
#include "struct.h"

class Map {
	public:
		Map();
        
		void initPheromoneMap(double initialPhValue);
		void evaporate();		
		
        void printPheromones();		

		void setEvaporationRate(double er) { evaporationRate = er; };		
		void setPhValue(int j, double p);
		void setMinPh(double p) { mMinPh = p; }
		void setMaxPh(double p) { mMaxPh = p; }

		double * pheromones;
        std::vector<P_Cont> *packets;
	private:
		
		double evaporationRate;
		double mMinPh = 0.0;
		double mMaxPh = 10.0;
};

#endif