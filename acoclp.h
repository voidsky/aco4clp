#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class ACOCLP {
public:
	bool init(const string filename);

	static double widCont, lenCont, heiCont, numPackets;
	static int numPacketsTotal;
	static int numPacketTypes;

};

