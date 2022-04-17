#include "clpdata.h"

double  ClpData::volumeCont,  ClpData::weiCont;
double  ClpData::widCont,  ClpData::lenCont,  ClpData::heiCont,  ClpData::numPackets;
double  ClpData::volumePackets;
int  ClpData::numPacketTypes;
int  ClpData::numPacketsTotal;

double*  ClpData::len;
double*  ClpData::wid;
double*  ClpData::hei;
double*  ClpData::wei;
int*  ClpData::orient;
int*  ClpData::number;
	
vector<int> ClpData::ori1;
vector<int> ClpData::ori2;
vector<int> ClpData::ori3;

bool ClpData::load(const string filename) {

	ifstream input(filename.c_str());
	if (!input.is_open()) {
		cerr << "Could not open file " << filename << endl;
	}
	
	input >> lenCont >> widCont >> heiCont >> weiCont >> numPacketTypes;
	volumeCont = lenCont * widCont * heiCont;
        
    cout << fixed << noshowpoint;
	//cout << "Container LxWxH and volume " << lenCont << " x " << widCont << " x " << heiCont << " is " << volumeCont << " cm3" << endl;
		
    len = new double[numPacketTypes];
    wid = new double[numPacketTypes];
    hei = new double[numPacketTypes];
    wei = new double[numPacketTypes];
    orient = new int[numPacketTypes];
    number = new int[numPacketTypes];

    ori1.resize(numPacketTypes);
    ori2.resize(numPacketTypes);
    ori3.resize(numPacketTypes);
    
    numPacketsTotal = 0;
    volumePackets = 0.0;

	for (int i = 0; i < numPacketTypes; i++)
	{
		input >> len[i] >> ori1[i] >> wid[i] >> ori2[i] >> hei[i] >> ori3[i] >> wei[i] >> number[i];
		orient[i] = 2 * ori1[i] + 2 * ori2[i] + 2 * ori3[i];		
		numPacketsTotal += number[i];
		volumePackets += len[i] * wid[i] * hei[i] * number[i]; 
		
		if (wid[i] > widCont || len[i] > lenCont || hei[i] > heiCont) {
			cerr << " Error: size of box is bigger than width of container "<< endl;
			input.close();
			return false;
		}
	}

    //cout << "Total packet volume " << volumePackets  << " cm3 " << endl;
	//cout << "Packet types " << numPacketTypes << ", total packet number " << numPacketsTotal << endl;
	
    input.close();

    return true;
}
