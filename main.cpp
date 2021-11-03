#include "clp.h"
#include "ant.h"
#include "map.h"
#include <stdlib.h> 
#include <time.h> 
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool compareByVolume(const P_Cont &a, const P_Cont &b)
{
    return a.volume < b.volume;
}

void testSorted(Map& map, CLP& clp)
{
	vector<Node> sortedNodes;		
	vector<P_Cont> copy = *map.packets;
	std::sort(copy.begin(), copy.end(), compareByVolume);
	for (size_t i = 0; i < copy.size(); i++)
	{
			Node n;
			n.ptrToMapNode = &copy.at(i);
			n.typeId = n.ptrToMapNode->typeId;
			n.orientation = 0;			
			n.count = 1;
			sortedNodes.push_back(n);
	}
	double *volume = new double();
	double *weight = new double();				
	clp.evaluateIndividual(&sortedNodes, volume, weight);
	double sur = *volume / 100;
	cout << "Sorted Volume = " << *volume << "% " << "Weight = " << *weight << " sur " << sur <<  endl;
	return;
}

int main(void) {
	srand(time(NULL));
	ANTOBJECTIVE currentObj = OBJ_VOLUME;

	CLP clp;
	Map map; 			
	clp.init("Dereli.dat");	
	map.packets = clp.packets;
	map.initPheromoneMap(3);
	map.setEvaporationRate(0.1);
	map.setMinPh(1);	
	map.setMaxPh(5);

	//testSorted(map, clp);

for (int itterations = 0; itterations < 100; itterations++)
{
	/* Creating ants and choosing first node */
	vector<Ant> * ants = new vector<Ant>;
	for (int i = 0; i < 20; i++)
	{
		CLP antsClp;
		antsClp = clp;	
		Ant a(clp, map, clp.volumeCont, clp.weiCont, currentObj);			
		a.chooseFirst();		
		ants->push_back(a);		
	}


	double highestSur = 0.0;
	Ant * bestAnt;

	bool wasChosen;
	do
	{
		wasChosen = false;
		for (size_t i = 0; i < ants->size(); i++)
		{
			Ant * a = &ants->at(i);
			if (a->chooseNext() == false )
			{		
				double sur = a->getSur();				
				//a->updatePheromonePath(sur);

				if (sur > highestSur) 
				{
					highestSur = sur;
					bestAnt = a;
				}
			} else {
				wasChosen = true;
			}
		}
		// all ants made their step, evaporate map
		map.evaporate();

	} while (wasChosen);
	

	cout << "Best ant path size " << bestAnt->mPath->size() << 
		" Ant sur " << bestAnt->getSur() << 
		" path vol " << (bestAnt->mPathVolume * 100) / bestAnt->mMaxContainerVolume << 
		" path wei " << (bestAnt->mPathWeight * 100) / bestAnt->mMaxContainerWeight << endl; 
	bestAnt->updatePheromonePath(bestAnt->getSur()/10);
	//map.evaporate();
	delete ants;

}

	//clp.printIndividual(ant.getPath());




} 
