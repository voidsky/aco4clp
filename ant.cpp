#include "ant.h"
using namespace std;

Ant::Ant(vector<P_Cont> *globalMap) 
		{
			path = new vector<Node>();

			for (size_t i = 0; i < globalMap->size(); i++)
				ptrsToGlobalMap.push_back(&globalMap->at(i));				
			
		}

bool Ant::grabNode() 
		{
			if (ptrsToGlobalMap.empty()) return false;
			// pick item from copy of global map leftovers
			int itemIndex = uniformRandom(ptrsToGlobalMap.size());

			// add it to ants path
			
			Node n;
			n.ptrToMapNode = ptrsToGlobalMap.at(itemIndex);
			n.typeId = n.ptrToMapNode->typeId;
			n.orientation = uniformRandom(n.ptrToMapNode->orientations);			
			n.count = 1;
			path->push_back(n);

			// remove it from our global map leftovers
			ptrsToGlobalMap.erase( ptrsToGlobalMap.begin() + itemIndex );

			return true;

		}

vector<Node> * Ant::getPath() 
		{
			return path;
		}
