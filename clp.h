#ifndef CLP_H
#define CLP_H

#include "struct.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Gestión de huecos
class Holes {

	public: 
		Holes();
		Holes(double H_wid, double H_len, double H_hei, double H_vol); //, int Posx, int Posy, int Posz);
		inline double getWid() {return H_wid;}
		inline double getLen() {return H_len;}
		inline double getHei() {return H_hei;}
		inline double getVol() const {return H_vol;}
		inline double getPosx() {return Posx;}
		inline double getPosy() {return Posy;}
		inline double getPosz() {return Posz;}		
		
	private:
		double H_wid;
		double H_len;
		double H_hei;
		double H_vol;
		int Posx;
		int Posy;
		int Posz;
};

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

	vector<P_Cont> *packets;
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