#ifndef ANT_H
#define ANT_H

#include "struct.h"

class Ant {
	public:
		Ant(std::vector<P_Cont> *globalMap);
		bool grabNode();
        std::vector<Node> * getPath();

	private:
		std::vector<P_Cont*> ptrsToGlobalMap;
		std::vector<Node> *path;	
};

#endif 