#include "clp.h"
#include "ant.h"
#include <stdlib.h> 
#include <time.h> 
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

int main(void) {
	srand(time(NULL));
	
	CLP clp;
	Map * map = new Map();   			
	clp.init("Dereli.dat", map->packets);	
	map->initPheromoneMap(2);
	map->setEvaporationRate(0.001);

	/* Creating ants and choosing first node */
	vector<Ant> * ants = new vector<Ant>;
	for (int i = 0; i < 1000; i++)
	{
		Ant a(&clp, map, clp.volumeCont);			// create ant
		a.chooseFirst();		// choose first packet in packet map
		ants->push_back(a);		// add ant to ants list
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
				//get objective received
				double *volume = new double();
				double *obj2 = new double();				
				clp.evaluateIndividual(a->getPath(), volume, obj2);
				double sur = *volume / 100;		// calculate space utilisation ratio
				//cout << "Ant id finished " << a << " Volume = " << *volume << "% " << "Weight = " << *obj2 << " sur " << sur <<  endl;
				a->setSur(sur);
				//update pheromone path
				a->updatePheromonePath(sur);
				//cout << "Ant path items" << a->getPath()->size() << " Ant path volume " << a->getPathVolume() << endl;
				//while (cin.get() != '\n') {}
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
		map->evaporate();

	} while (wasChosen);
	

	cout << "Ant path items" << bestAnt->getPath()->size() << " Ant path volume " << bestAnt->getPathVolume() << " ant sur " << bestAnt->getSur() << endl;


	//clp.printIndividual(ant.getPath());




} 
