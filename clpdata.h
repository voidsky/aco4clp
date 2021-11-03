#ifndef CLPDATA_H
#define CLPDATA_H

#include "struct.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;


class ClpData {
public:
	bool load(const string filename);
	
    static double volumeCont, weiCont;
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
};


#endif 