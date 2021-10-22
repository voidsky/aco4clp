#include "acoclp.h"

double ACOCLP::lenCont;
double ACOCLP::widCont;
double ACOCLP::heiCont;
int ACOCLP::numPacketTypes;

bool ACOCLP::init(const string filename) {
	ifstream input(filename.c_str());
	if (!input.is_open()) {
		cerr << "Could not open file " << filename << endl;
	}
	input >> lenCont >> widCont >> heiCont >> numPacketTypes;


	cout << "Container LxWxH " << lenCont << "x" << widCont << "x" << heiCont << endl;
	cout << "Packet types " << numPacketTypes << endl;
	

	return true;
}
