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
	
	
   	vector<P_Cont> * packets = new vector<P_Cont>();
	CLP clp;
	clp.init("Dereli.dat", packets);

	double ** pheromones;
    pheromones = new double*[packets->size()];
    for (size_t i = 0; i < packets->size(); i++)        
        {
                pheromones[i] = new double[packets->size()];
                for (size_t j = 0; j < packets->size(); j++) pheromones[i][j] = 1;                                        
        }



	vector<Ant> * ants;
	ants = new vector<Ant>;
	for (int i = 0; i < 5; i++)
	{
		Ant a(packets, pheromones);
		a.chooseFirst();
		ants->push_back(a);
	}

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
				double *obj1 = new double();
				double *obj2 = new double();				
				clp.evaluateIndividual(a->getPath(), obj1, obj2);
				cout << "And id finished " << a << " Volume = " << *obj1 << " " << "Weight = " << *obj2 << endl;
				//update pheromone path

			} else {
				wasChosen = true;
			}
		}
		//evaporate;

	} while (wasChosen);
	
		



	//clp.printIndividual(ant.getPath());




} 
