#ifndef MAP_H
#define MAP_H

#include "sys/time.h"
#define cimg_OS 0
#include "cimg/CImg.h"
using namespace cimg_library;

#include "struct.h"
#include "clpdata.h"

class Map {
	public:
		Map(ClpData& data);
        
		void initPheromones(double initialPhValue);
		void evaporate();	
		void saveHeatMap(const string filename);
		
        void printPheromones();		

		double getPhValue(int i, int j, int o);
		void setPhValue(int i,int j, int o, double p);

		double getNumberOfPackets() { return mPackets.size();};
		Node getPacket(int n) { return mPackets.at(n); };

		void setEvaporationRate(double er) { evaporationRate = er; };		
		void setMinPh(double p) { mMinPh = p; }
		void setMaxPh(double p) { mMaxPh = p; }

		ClpData mData;
	private:		
		double evaporationRate;
		double mMinPh = 0.0;
		double mMaxPh = 10.0;

		
		double *** mPheromones;
        std::vector<Node> mPackets;		
};

#endif