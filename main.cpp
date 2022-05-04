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

//#define RunRandom
//#define RunInOrder
//#define RunAS
//#define RunElitist1
//#define RunElitist2
//#define RunMaxMin
#define RunACS


double cacluclateRandom(const string filename, bool printPath) {
	
	ClpData data;
	if (!data.load(filename)) return -1;

	Map map(data);	
	Container c(data);
	Ant a(map, c);			
	a.chooseFirst();		
	while(a.chooseFirst()) {}

	cout << " Random ant path size " << a.getPathSteps() << " Ant sur " << a.getSur() << " path vol " << a.getVolumePercentage() << endl;
	if (printPath) a.printPathSteps();
	cout << endl;
	return a.getVolumePercentage();
}

double cacluclateInOrder(const string filename, bool printPath) {
	
	ClpData data;
	if (!data.load(filename)) return -1;

	Map map(data);	
	Container c(data);
	Ant a(map, c);			
	a.chooseInOrder();

	cout << " Ordered ant path size " << a.getPathSteps() << " Ant sur " << a.getSur() << " path vol " << a.getVolumePercentage() << endl;
	if (printPath) a.printPathSteps();
	cout << endl;
	return a.getVolumePercentage();
}

double cacluclateAS(const string filename, bool printPath) {

	ClpData data;
	if (!data.load(filename)) return -1;

	Map map(data);	
	map.setEvaporationRate(0.1);
	map.setMinPh(-1);	
	map.setMaxPh(-1);
	map.initPheromones(8);
	int itterationAntCount = 100;

	int notImprovedCount = 0;
	Ant * globallyBestAnt;
	double globallyHighestSur = 0.0;

	int totalIteration = 0;
	
	while (notImprovedCount<30)
	{	
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
		}while (wasChosen);

		map.evaporate();
		itterationBestAnt->updatePheromonePath(itterationBestAnt->getSur());
		

		if (ittertationHighestSur > globallyHighestSur) {			
			globallyHighestSur = ittertationHighestSur; 
			globallyBestAnt=itterationBestAnt; 
			notImprovedCount = 0;
		} else 
		{			
			notImprovedCount++;
		};	

		cout << notImprovedCount << " AS Itteration best ant path size " << itterationBestAnt->getPathSteps() << " Ant sur " << itterationBestAnt->getSur() << " path vol " << itterationBestAnt->getVolumePercentage() << endl;
		//map.saveHeatMap("./heatmap/ASheatmap"+std::to_string(totalIteration)+".png");
		totalIteration++;
	}
		
	//map.saveHeatMap("./heatmap/ASheatmapFinal.png");
	cout << "AS Globally best ant path size " << globallyBestAnt->getPathSteps() << " Ant sur " << globallyBestAnt->getSur() << " path vol " << globallyBestAnt->getVolumePercentage() << endl;
	if (printPath) globallyBestAnt->printPathSteps();
	cout << endl;
	return globallyBestAnt->getVolumePercentage();
}

double cacluclateElitist1(const string filename, bool printPath) {

	ClpData data;
	if (!data.load(filename)) return -1;

	Map map(data);	
	map.setEvaporationRate(0.1);
	map.setMinPh(-1);	
	map.setMaxPh(-1);
	map.initPheromones(8);
	int itterationAntCount = 100;

	int notImprovedCount = 0;
	Ant * globallyBestAnt;
	double globallyHighestSur = 0.0;

	int totalIteration = 0;
	
	while (notImprovedCount<30)
	{	
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

		}while (wasChosen);
	
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

		cout << notImprovedCount << " Elisitst1 Itteration best ant path size " << itterationBestAnt->getPathSteps() << " Ant sur " << itterationBestAnt->getSur() << " path vol " << itterationBestAnt->getVolumePercentage() << endl;		
		//map.saveHeatMap("./heatmap/Elitist1heatmap"+std::to_string(totalIteration)+".png");
		totalIteration++;
	}
		
	//map.saveHeatMap("./heatmap/Elitist1heatmapFinal"+to_string(time(NULL))+".png");
	cout << " Elisitst1 Globally best ant path size " << globallyBestAnt->getPathSteps() << " Ant sur " << globallyBestAnt->getSur() << " path vol " << globallyBestAnt->getVolumePercentage() << endl;
	if (printPath) globallyBestAnt->printPathSteps();
	cout << endl;
	return globallyBestAnt->getVolumePercentage();
}

double cacluclateElitist2(const string filename, bool printPath) {
	
	ClpData data;
	if (!data.load(filename)) return -1;

	Map map(data);	
	map.setEvaporationRate(0.1);
	map.setMinPh(-1);	
	map.setMaxPh(-1);
	map.initPheromones(8);
	int itterationAntCount = 100;

	int notImprovedCount = 0;
	double globallyHighestSur = 0.0;

	int totalIteration = 0;
	
	Ant * globallyBestAnt;

	while (notImprovedCount<20)
	{	
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
		}while (wasChosen);
	
		map.evaporate();

		if (ittertationHighestSur > globallyHighestSur) {			
			globallyHighestSur = ittertationHighestSur; 
			globallyBestAnt=itterationBestAnt; 
			notImprovedCount = 0;
		} else 
		{			
			notImprovedCount++;
		};	

		itterationBestAnt->updatePheromonePathElitist(globallyBestAnt);

		cout << notImprovedCount << " Elisitst2 Itteration best ant path size " << itterationBestAnt->getPathSteps() << " Ant sur " << itterationBestAnt->getSur() << " path vol " << itterationBestAnt->getVolumePercentage() << endl;
		//map.saveHeatMap("./heatmap/heatmap"+std::to_string(totalIteration)+".png");
		totalIteration++;
	}
		
	map.saveHeatMap("./heatmap/Elitist2heatmapFinal"+to_string(time(NULL))+".png");
	cout << "Elisitst2 Globally best ant path size " << globallyBestAnt->getPathSteps() << " Ant sur " << globallyBestAnt->getSur() << " path vol " << globallyBestAnt->getVolumePercentage() << endl;
	if (printPath) globallyBestAnt->printPathSteps();
	cout << endl;
	return globallyBestAnt->getVolumePercentage();
}

double cacluclateMaxMin(const string filename, bool printPath) {
	
	ClpData data;
	if (!data.load(filename)) return -1;

	Map map(data);	
	map.setEvaporationRate(0.2);
	map.setMinPh(10);	
	map.setMaxPh(20);
	map.initPheromones(20);
	int itterationAntCount = 100;

	int notImprovedCount = 0;
	double globallyHighestSur = 0.0;

	int totalIteration = 0;
	
	Ant * globallyBestAnt;

	while (notImprovedCount<30)
	{	
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
		}while (wasChosen);
	
		map.evaporate();

		/*if (ittertationHighestSur > globallyHighestSur && globallyHighestSur > 0.0) {
			itterationBestAnt->updatePheromonePath(itterationBestAnt->getSur() * 1.5);
		}
		else {
			itterationBestAnt->updatePheromonePath(itterationBestAnt->getSur());
		}*/

		if (ittertationHighestSur > globallyHighestSur) {			
			globallyHighestSur = ittertationHighestSur; 
			globallyBestAnt=itterationBestAnt; 
			notImprovedCount = 0;
		} else 
		{			
			notImprovedCount++;
		};	

		globallyBestAnt->updatePheromonePath(globallyBestAnt->getSur());

		cout << notImprovedCount << " Max Min Itteration best ant path size " << itterationBestAnt->getPathSteps() << " Ant sur " << itterationBestAnt->getSur() << " path vol " << itterationBestAnt->getVolumePercentage() << endl;

		//map.saveHeatMap("./heatmap/heatmap"+std::to_string(totalIteration)+".png");

		totalIteration++;
	}
		
	//map.saveHeatMap("./heatmap/MaxMinheatmapFinal"+to_string(time(NULL))+".png");
	cout << "Max Min Globally best ant path size " << globallyBestAnt->getPathSteps() << " Ant sur " << globallyBestAnt->getSur() << " path vol " << globallyBestAnt->getVolumePercentage() << endl;
	if (printPath) globallyBestAnt->printPathSteps();
	cout << endl;
	return globallyBestAnt->getVolumePercentage();
}

double cacluclateACS(const string filename, bool printPath) {
	
	ClpData data;
	if (!data.load(filename)) return -1;

	Map map(data);	
	map.setEvaporationRate(0.1);
	map.setMinPh(4);	
	map.setMaxPh(8);
	map.initPheromones(8);
	int itterationAntCount = 100;

	int notImprovedCount = 0;
	double globallyHighestSur = 0.0;

	int totalIteration = 0;
	
	Ant * globallyBestAnt;

	while (notImprovedCount<20)
	{	
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
				
				bool chosen = false;

				double q = randfrom(0,1);
				double q0 = 0.5;	

				double alpha = 1.0;
				double beta = 1.0;
				/*if (q <= q0) 
					chosen = a->chooseNextACS(1.0, 1.0, true);
				else
					chosen = a->chooseNext(1.0, 1.0, true);*/

				//a->updateAfterOneStepForACS(0.1, a->getSur());

				if (q <= q0) {
					beta = randfrom(1,5);
				}
				chosen = a->chooseNext(alpha, beta , false);

				if (chosen == false )
				{		
					double sur = a->getSur();
					a->printPathSteps();				
					if (sur > ittertationHighestSur) 
					{
						ittertationHighestSur = sur;
						itterationBestAnt = a;
					}
				} else {
					wasChosen = true;					
				}
			}
		}while (wasChosen);

		

		/*if (ittertationHighestSur > globallyHighestSur && globallyHighestSur > 0.0) {
			itterationBestAnt->updatePheromonePath(itterationBestAnt->getSur() * 1.5);
		}
		else {
			itterationBestAnt->updatePheromonePath(itterationBestAnt->getSur());
		}*/		
	
		if (ittertationHighestSur > globallyHighestSur) {			
			globallyHighestSur = ittertationHighestSur; 
			globallyBestAnt=itterationBestAnt; 
			notImprovedCount = 0;
		} else 
		{			
			notImprovedCount++;
		};	

		map.evaporate();
		globallyBestAnt->updatePheromonePath(globallyBestAnt->getSur());
	
		cout << notImprovedCount << " ACS Itteration best ant path size " << itterationBestAnt->getPathSteps() << " Ant sur " << itterationBestAnt->getSur() << " path vol " << itterationBestAnt->getVolumePercentage() << endl;
		//cout << "Globally best ant " << globallyBestAnt << endl;

		if (notImprovedCount >0 && notImprovedCount%10 == 0) {
			cout << "10 iterations no improvement - reinitiate map" << endl;
			map.initPheromones(8);
		}
		//map.saveHeatMap("./heatmap/heatmap"+std::to_string(totalIteration)+".png");

		totalIteration++;
	}
		
	//map.saveHeatMap("./heatmap/MaxMinheatmapFinal"+to_string(time(NULL))+".png");
	cout << "ACS Globally best ant path size " << globallyBestAnt->getPathSteps() << " Ant sur " << globallyBestAnt->getSur() << " path vol " << globallyBestAnt->getVolumePercentage() << endl;
	if (printPath) globallyBestAnt->printPathSteps();
	cout << endl;
	return globallyBestAnt->getVolumePercentage();
}

int main(void) {
	srand (time(NULL));

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
	const int count = 20;

#ifdef RunInOrder
	double resultsInOrder[count];
	double sumInOrder, averageInOrder;
	for	(int n = 0; n<count; n++) {
		cout << "Run " << n << endl;
		double res = cacluclateInOrder("Dereli.dat", true);
		resultsInOrder[n] = res;
		sumInOrder += res;
	}
	averageInOrder = sumInOrder / count;

	for (int n=0; n<count;n++) cout << "("<< n << "," << resultsInOrder[n] << ")";
	cout << endl << "Min:" << *min_element(resultsInOrder, resultsInOrder + count) << ", Max:" << *max_element(resultsInOrder, resultsInOrder + count);
	cout << endl << "In order, average " << averageInOrder << endl;
#endif

#ifdef RunRandom
	double resultsRandom[count];
	double sumRandom, averageRandom;
	for	(int n = 0; n<count; n++) {
		cout << "Run " << n << endl;
		double res = cacluclateRandom("Dereli.dat", true);
		resultsRandom[n] = res;
		sumRandom += res;
	}
	averageRandom = sumRandom / count;

	for (int n=0; n<count;n++) cout << "("<< n << "," << resultsRandom[n] << ")";
	cout << endl << "Min:" << *min_element(resultsRandom, resultsRandom + count) << ", Max:" << *max_element(resultsRandom, resultsRandom + count);
	cout << endl << "Random, average " << averageRandom << endl;
#endif

#ifdef RunAS
	double resultsAS[count];
	double sumAS, averageAS;
	for	(int n = 0; n<count; n++) {
		cout << "Run " << n << endl;
		double res = cacluclateAS("Dereli.dat", true);
		resultsAS[n] = res;
		sumAS += res;
	}
	averageAS = sumAS / count;

	for (int n=0; n<count;n++) cout << "("<< n << "," << resultsAS[n] << ")";
	cout << endl << "Min:" << *min_element(resultsAS, resultsAS + count) << ", Max:" << *max_element(resultsAS, resultsAS + count);
	cout << endl << "AS, average " << averageAS << endl;
#endif

#ifdef RunElitist1
	double resultsElitist1[count];
	double sumElitist1, averageElitist1;
	for	(int n = 0; n<count; n++) {
		cout << "Run " << n << endl;
		double res = cacluclateElitist1("Dereli.dat", true);;
		resultsElitist1[n] = res;
		sumElitist1 += res;
	}
	averageElitist1 = sumElitist1 / count;

	for (int n=0; n<count;n++) cout << "("<< n << "," << resultsElitist1[n] << ")";
	cout << endl << "Min:" << *min_element(resultsElitist1, resultsElitist1 + count) << ", Max:" << *max_element(resultsElitist1, resultsElitist1 + count);	
	cout << endl << "Elitist1, average " << averageElitist1 << endl;
#endif

#ifdef RunElitist2
	double resultsElitist2[count];
	double sumElitist2, averageElitist2;
	for	(int n = 0; n<count; n++) {
		cout << "Run " << n << endl;
		double res = cacluclateElitist2("Dereli.dat", true);
		resultsElitist2[n] = res;
		sumElitist2 += res;
	}
	averageElitist2 = sumElitist2 / count;

	for (int n=0; n<count;n++) cout << "("<< n << "," << resultsElitist2[n] << ")";
	cout << endl << "Min:" << *min_element(resultsElitist2, resultsElitist2 + count) << ", Max:" << *max_element(resultsElitist2, resultsElitist2 + count);		
	cout << endl << "Elitist2, average " << averageElitist2 << endl;
#endif

#ifdef RunMaxMin
	double resultsMaxMin[count];
	double sumMaxMin, averageMaxMin;
	for	(int n = 0; n<count; n++) {
		cout << "Run " << n << endl;
		double res = cacluclateMaxMin("Dereli.dat", true);
		resultsMaxMin[n] = res;
		sumMaxMin += res;
	}
	averageMaxMin = sumMaxMin / count;

	for (int n=0; n<count;n++) cout << "("<< n << "," << resultsMaxMin[n] << ")";
	cout << endl << "Min:" << *min_element(resultsMaxMin, resultsMaxMin + count) << ", Max:" << *max_element(resultsMaxMin, resultsMaxMin + count);			
	cout << endl << "MaxMin, average " << averageMaxMin << endl;
#endif



#ifdef RunACS
	double resultsACS[count];
	double sumACS, averageACS;
	for	(int n = 0; n<count; n++) {
		cout << "Run " << n << endl;
		double res = cacluclateACS("Dereli.dat", true);
		resultsACS[n] = res;
		sumACS += res;
	}
	averageACS = sumACS / count;

	for (int n=0; n<count;n++) cout << "("<< n << "," << resultsACS[n] << ")";
	cout << endl << "Min:" << *min_element(resultsACS, resultsACS + count) << ", Max:" << *max_element(resultsACS, resultsACS + count);			
	cout << endl << "ACS, average " << averageACS << endl;
#endif


	

	




} 
