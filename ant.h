#ifndef ANT_H
#define ANT_H

#include "struct.h"

class Ant {
	public:
		Ant(std::vector<P_Cont> *packets, double ** ph);	
		bool chooseFirst();
		bool chooseNext();
        std::vector<Node> *getPath();

	private:
		bool addToPath(P_Cont * packet);
		std::vector<P_Cont*> *ptrsToGlobalMap;
		std::vector<Node> *path;	
		double ** pheromones;	
};

#endif 