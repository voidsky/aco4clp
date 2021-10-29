#include "ant.h"
using namespace std;

Ant::Ant(vector<P_Cont> *packets, double ** ph) 
{
	path = new vector<Node>();
	ptrsToGlobalMap = new vector<P_Cont*>();
	pheromones = ph;

	for (size_t i = 0; i < packets->size(); i++)
		ptrsToGlobalMap->push_back(&packets->at(i));				
		
}

bool Ant::chooseFirst() 
		{
			if (ptrsToGlobalMap->empty()) return false;

			int itemIndex = uniformRandom(ptrsToGlobalMap->size());			
			cout << "Ant starting from id " << ptrsToGlobalMap->at(itemIndex)->id << endl;
			addToPath(ptrsToGlobalMap->at(itemIndex));

			ptrsToGlobalMap->erase( ptrsToGlobalMap->begin() + itemIndex );
			return true;
		}

bool Ant::chooseNext() 
		{
			if (ptrsToGlobalMap->empty()) return false;

			// id of our last path node
			int thisId = path->back().ptrToMapNode->id;

			double sumAreaTimesPh = 0.0;
			for (size_t n = 0; n < ptrsToGlobalMap->size(); n++)
			{
				int j = ptrsToGlobalMap->at(n)->id;
				sumAreaTimesPh += ptrsToGlobalMap->at(n)->bottomArea * pheromones[thisId][j];
			}

			// itterate our local map copy
			double maxProbability = 0.0;
			int bestNextIndex;
			for (size_t n = 0; n < ptrsToGlobalMap->size(); n++)
			{
				int j = ptrsToGlobalMap->at(n)->id;
				double bottomArea =  ptrsToGlobalMap->at(n)->bottomArea;
				// get pheromone value of possible next step
				double phij = pheromones[thisId][j];
				double probability = (phij * bottomArea) / sumAreaTimesPh;
				if (probability > maxProbability) {
					maxProbability = probability;
					bestNextIndex = n;
				}
			}
			
			addToPath(ptrsToGlobalMap->at(bestNextIndex));
			ptrsToGlobalMap->erase( ptrsToGlobalMap->begin() + bestNextIndex );
			return true;
		}	

vector<Node> * Ant::getPath() 
		{
			return path;
		}

bool Ant::addToPath(P_Cont * packet) 
{
			// add it to ants path			
			Node n;
			n.ptrToMapNode = packet;
			n.typeId = n.ptrToMapNode->typeId;
			n.orientation = 0;			
			n.count = 1;
			path->push_back(n);
			return true;
}
