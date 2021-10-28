
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
	
   	vector<P_Cont> *globalMap = new vector<P_Cont>();

	CLP clp;
	clp.init("Dereli.dat", globalMap);
	
	Ant ant(globalMap);
	while (ant.grabNode()) {};

	//clp.printIndividual(ant.getPath());

	double *obj1 = new double();
	double *obj2 = new double();
	clp.evaluateIndividual(ant.getPath(), obj1, obj2);

	// Print objective values
	cout << "Volume = " << *obj1 << endl;
	cout << "Weight = " << *obj2 << endl;

} 
