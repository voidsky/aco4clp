#include "ant.h"
#include "map.h"
#include "clpdata.h"

#include <stdlib.h> 
#include <time.h> 
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


int main(void) {
	srand(time(NULL));
	
	ClpData data;
	if (!data.load("Dereli.dat")) return -1;

	Map map(data);
	map.initPheromones(2);
	map.setEvaporationRate(0.1);
	map.setMinPh(1);	
	map.setMaxPh(3);

	ANTOBJECTIVE currentObj = OBJ_VOLUME;

	for (int itterations = 0; itterations < 100; itterations++)
	{
		vector<Ant> * ants = new vector<Ant>;
		for (int i = 0; i < 20; i++)
		{	
			Container c(data);
			Ant a(map, c, currentObj);			
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
			//map.evaporate();
		} while (wasChosen);
	
		cout << "Best ant path size " << bestAnt->getPathSteps() << 
			" Ant sur " << bestAnt->getSur() << 
			" path vol " << bestAnt->getVolumePercentage() << 
			" path wei " << bestAnt->getWeightPercentage() << endl; 
		bestAnt->updatePheromonePath(bestAnt->getSur()/10);
		map.evaporate();
		delete ants;

	}



} 
