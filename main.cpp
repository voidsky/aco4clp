#include "ant.h"
#include "map.h"
#include "clpdata.h"

#include "helpers.h"
#include <omp.h>

#include <stdlib.h> 
#include <time.h> 
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

double cacluclate(const string filename, bool printPath) {
	
	srand (time(NULL));

	ClpData data;
	if (!data.load(filename)) return -1;

	Map map(data);	
	map.setEvaporationRate(0.1);
	map.setMinPh(1);	
	map.setMaxPh(20);
	map.initPheromones(20);
	int itterationAntCount = 100;

	int notImprovedCount = 0;
	Ant * globallyBestAnt;
	double globallyHighestSur = 0.0;

	int totalIteration = 0;
	
	while (notImprovedCount<10)
	{	
		//Timer t;
		//t.Start();

		// initialize N ants, put them on map
		vector<Ant> * ants = new vector<Ant>;		
		for (int i = 0; i < itterationAntCount; i++)
		{	
			Container c(data);
			Ant a(map, c);			
			a.chooseFirst();		
			ants->push_back(a);
		}				

		bool wasChosen;		
		Ant * itterationBestAnt;
		double ittertationHighestSur = 0.0;		
		do
		{
			wasChosen = false;
			#pragma omp parallel for 
			for (size_t i = 0; i < ants->size(); i++)
			{
				Ant * a = &ants->at(i);
				if (a->chooseNext(1.0, 1.0, true) == false )	//weigh of phoromones alpha versus volume beta, alpha = 1, beta = [2,...,5]
				{		
					double sur = a->getSur();				
					if (sur > ittertationHighestSur) 
					{
						ittertationHighestSur = sur;
						itterationBestAnt = a;
					}
				} else {
					wasChosen = true;
				}
			}
//			map.evaporate();
//			if (itterationBestAnt) itterationBestAnt->updatePheromonePath(itterationBestAnt->getSur());
		}while (wasChosen);
	
		/*for (size_t i = 0; i < ants->size(); i++)
		{
			ants->at(i).printPathSteps();
		}*/

		map.evaporate();

		if (ittertationHighestSur > globallyHighestSur && globallyHighestSur > 0.0) {
			itterationBestAnt->updatePheromonePath(itterationBestAnt->getSur() * 1.5);
		}
		else {
			itterationBestAnt->updatePheromonePath(itterationBestAnt->getSur());
		}

		if (ittertationHighestSur > globallyHighestSur) {			
			globallyHighestSur = ittertationHighestSur; 
			globallyBestAnt=itterationBestAnt; 
			notImprovedCount = 0;
		} else 
		{			
			notImprovedCount++;
		};	

		cout << notImprovedCount << " Itteration best ant path size " << itterationBestAnt->getPathSteps() << " Ant sur " << itterationBestAnt->getSur() << " path vol " << itterationBestAnt->getVolumePercentage() << endl;
		//t.StopAndPrint();

		map.saveHeatMap("./heatmap/heatmap"+std::to_string(totalIteration)+".png");

		totalIteration++;
	}
		
	map.saveHeatMap("./heatmap/heatmapFinal.png");

	cout << "Globally best ant path size " << globallyBestAnt->getPathSteps() << " Ant sur " << globallyBestAnt->getSur() << " path vol " << globallyBestAnt->getVolumePercentage() << endl;
	if (printPath) globallyBestAnt->printPathSteps();
	//cout << "\r Itterations with no improvement " << notImprovedCount;	
	cout << endl;

	return globallyBestAnt->getSur();
}

int main(void) {

	// test 1 - resut should be 1
	//cout << "Test 1 ";
	//cacluclate("test1.dat", true);
	
	// test 2 - resut should be 1
	//cout << "Test 2 ";
	//cacluclate("test2.dat", true);

	// test 3 - resut should be 0.515625
	//cout << "Test 3 ";
	//cacluclate("test3.dat", true);
	//cacluclate("test4.dat", true);

	// 
	//cout << "Dereli.dat ";
	cacluclate("Dereli.dat", true);

} 
