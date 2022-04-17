#ifndef CLP_H
#define CLP_H

#include "struct.h"
#include "container.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;


class CLP {
public:
	bool init(const string filename);

	void printIndividual(vector<Node> *g);	
	void evaluateIndividual (vector<Node> *g, double *o1, double *o2);
	void evaluateIndividual2 (double &o1, double &o2);
	void AddNode(Node n);
	void RemoveLast(double&  o1, double& o2);
	void orientation_pieces(int id, int rot);
	static double volumeCont, weiCont;

	vector<Node> *packets;
	bool AllPacked();	
private:
	static double widCont, lenCont, heiCont, numPackets;
	static double volumePackets;
	static int numPacketTypes;
	static int numPacketsTotal;

	static double *len;
	static double *wid;
	static double *hei;
	static double *wei;
	static int *orient;
	static int *number;

	static vector<int> ori1;
	static vector<int> ori2;
	static vector<int> ori3;	

	vector<P_Order> items;    // Items ordenados segun la heuristica
	vector<P_Order> itemsCont;

	vector<Holes> holesF;       // Vector de huecos Front dentro del contenedor
	vector<Holes> holesA;       // Vector de huecos Above dentro del contenedor
	vector<Holes> holesB;       // Vector de huecos Beside dentro del contenedor

	int widR, depR, heiR;

	int mW_Cont_free = 0.0;
};


#endif 