#ifndef MAP_H
#define MAP_H

#include "struct.h"
#include "clpdata.h"

class Map {
	public:
		Map(ClpData& data);
        
		void initPheromones(double initialPhValue);
		void evaporate();		
		
        void printPheromones();		

		double getPhValue(int id) { return mPheromones[id]; };
		void setPhValue(int j, double p);

		double size() { return mPackets->size();};
		P_Cont * getPacketPtr(int n) { return &mPackets->at(n); }

		void setEvaporationRate(double er) { evaporationRate = er; };		
		void setMinPh(double p) { mMinPh = p; }
		void setMaxPh(double p) { mMaxPh = p; }


	private:		
		double evaporationRate;
		double mMinPh = 0.0;
		double mMaxPh = 10.0;

		ClpData mData;
		double * mPheromones;
        std::vector<P_Cont> *mPackets;		
};

#endif