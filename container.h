#ifndef CONTAINER_H
#define CONTAINER_H

#include "struct.h"
#include "clpdata.h"
#include <algorithm>
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

class Container {
public:
    Container(ClpData& data) { mData = data; lenCont = data.lenCont; widCont = data.widCont; heiCont = data.heiCont; volume = data.volumeCont; weight = data.weiCont; mW_Cont_free = data.widCont; }
	bool evaluateIndividual2 (Node n, double &o1, double &o2);
	void AddNode(Node n);
	void RemoveLast();
	void orientation_pieces(int id, int rot,double& widR,double& depR,double& heiR);
	vector<Node> *packets;
	bool AllPacked();
    double volume, weight;	
private:

    static  bool cmpVol(const Holes &h1, const Holes &h2) { return (h1.getVol() < h2.getVol()); };
    ClpData mData;
	vector<P_Order> items;    // Items ordenados segun la heuristica
	vector<P_Order> itemsCont;

	vector<Holes> holesF;       // Vector de huecos Front dentro del contenedor
	vector<Holes> holesA;       // Vector de huecos Above dentro del contenedor
	vector<Holes> holesB;       // Vector de huecos Beside dentro del contenedor

    double lenCont, heiCont, widCont;

	int mW_Cont_free = 0.0;
};


#endif