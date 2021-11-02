#include "CLP.h"
#include <vector>
#include <stack>
#include <set>
#include <algorithm>
#include <iomanip>

double CLP::lenCont;
double CLP::widCont;
double CLP::heiCont;
double CLP::weiCont;
double CLP::volumeCont;
double CLP::volumePackets;
int CLP::numPacketTypes;
int CLP::numPacketsTotal;
double* CLP::len;
double* CLP::wid;
double* CLP::hei;
double* CLP::wei;
int* CLP::orient;
int* CLP::number;
vector<int> CLP::ori1;
vector<int> CLP::ori2;
vector<int> CLP::ori3;

// ------------------------------------------------------------------------------
// Funcion auxiliar para la ordenacion de los huecos de menor a mayor volumen
// -----------------------------------------------------------------------------
bool cmpVol(const Holes &h1, const Holes &h2) {
        return (h1.getVol() < h2.getVol());     
}

// *************************************************************************************
// Clase Holes. 
// Controla los huecos que se van formando dentro del contenedor
// *************************************************************************************
Holes::Holes() {
        H_wid = 0.0;
        H_len = 0.0;
        H_hei = 0.0;
        H_vol = 0.0;
}


Holes::Holes(double H_wid, double H_len, double H_hei, double H_vol) {
        this->H_wid = H_wid;
        this->H_len = H_len;
        this->H_hei = H_hei;
        this->H_vol = H_vol;
}

bool CLP::init(const string filename, vector<P_Cont>* packets) {
	ifstream input(filename.c_str());
	if (!input.is_open()) {
		cerr << "Could not open file " << filename << endl;
	}
	
	input >> lenCont >> widCont >> heiCont >> weiCont >> numPacketTypes;
	volumeCont = lenCont * widCont * heiCont;
        
        cout << fixed << noshowpoint;
	cout << "Container LxWxH and volume " << lenCont << " x " << widCont << " x " << heiCont << " is " << volumeCont << " cm3" << endl;
		
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


        cout << "Total packet volume " << volumePackets  << " cm3 " << endl;
	cout << "Packet types " << numPacketTypes << ", total packet number " << numPacketsTotal << endl;
	input.close();
        
        int id = 0;
        // our map is a vector of packets
        for (int type = 0; type < numPacketTypes; type++) 
                for (int packet = 0; packet < number[type]; packet++)
                {                
                                        P_Cont n;
                                        n.id = id++;
                                        n.typeId = type;
                                        n.orientations = orient[type];
                                        n.bottomArea = wid[type] * len[type];
                                        n.volume = wid[type] * len[type] * hei[type];
                                        packets->push_back(n);
                }
        

	return true;
}


//-------------------------------------------------------------------------------
// Print a candidate solution
// -----------------------------------------------------------------------------
void CLP::printIndividual(vector<Node> *g) { 
        cout  << setw(10) << "Type" << setw(10) << "Count" << setw(15) << "Orientation" << setw(10) << endl;
   for (unsigned int i = 0; i < g->size(); i++)
   	cout << setw(10) << (*g)[i].typeId << setw(10) << (*g)[i].count << setw(15) << (*g)[i].orientation << endl;
   cout << endl;
}


//-------------------------------------------------------------------------------
// Evaluate
// ------------------------------------------------------------------------------
void CLP::evaluateIndividual (vector<Node> *g, double *o1, double *o2) {

        // Variables para controlar las dimensiones de los huecos
        double Hwid;
        double Hlen;
        double Hhei;
        double Hvol;
        
        // Variables para ir acumulando los objetivos del problema
        (*o1) = (*o2) = 0;   // Valores de los objetivos Obj1->Volumen Obj2->Peso
            
        int W_Cont_free = widCont;   // Contador de wid libre en el contenedor
        double LDB;              // LDB es el ancho del nivel
        unsigned int count = 0;       // Indice dentro del vector itemsCont
        unsigned int holesId = 0;         // Indice dentro de los vectores de huecos
        bool label_control;

        itemsCont.resize(numPacketsTotal);
        int total, cont, id, rot;
        total = cont = 0;       
        
        for (unsigned int p = 0; p < g->size(); p++) {
                id = (*g)[p].typeId;      // Identificador de la pieza
                cont = (*g)[p].count; // Numero de piezas del mismo tipo
                rot = (*g)[p].orientation;  // Tipo de rotacion
                orientation_pieces(id, rot);
                for (int j = total; j < (total + cont); j++) {
                        itemsCont[j].wid = widR;
                        itemsCont[j].len = depR;
                        itemsCont[j].hei = heiR;
                        itemsCont[j].wei = wei[id];
                        itemsCont[j].vol = itemsCont[j].wid * itemsCont[j].hei * itemsCont[j].len;
                        itemsCont[j].pack = false;
                        itemsCont[j].index = j;
                }
                total += cont;
        }
        while ((count < itemsCont.size()) && (((*o2) + itemsCont[count].wei) <= weiCont) && (((*o1) + itemsCont[count].vol) <= volumeCont)){    

                // Se recorre primero el vector de holesF
                while ((holesId < holesF.size()) && (label_control != true)) {
                        if ((holesF[holesId].getWid() == itemsCont[count].wid) &&
                            (holesF[holesId].getLen() == itemsCont[count].len) &&
                            (holesF[holesId].getHei() == itemsCont[count].hei)) { // si cabe justo
                                holesF.erase(holesF.begin() + holesId);  // Se elimina el hueco del vector
                                itemsCont[count].pack = true;
                                (*o2) += itemsCont[count].wei;          // Se suma el peso
                                (*o1) += itemsCont[count].vol;         // Se suma el volumen
                                label_control = true;
                        }
                        else if ((holesF[holesId].getWid() >= itemsCont[count].wid) &&
                            (holesF[holesId].getLen() >= itemsCont[count].len) &&
                            (holesF[holesId].getHei() >= itemsCont[count].hei)) { // si sobra espacio, se deben crear los nuevos huecos
                                itemsCont[count].pack = true;
                                (*o2) += itemsCont[count].wei;          // Se suma el peso
                                (*o1) += itemsCont[count].vol;         // Se suma el volumen
                                label_control = true;
                                // Calcular huecos
                                if ((holesF[holesId].getWid() - itemsCont[count].wid) > 0) {  //Se crea hueco Beside
                                        Hwid = holesF[holesId].getWid() - itemsCont[count].wid;
                                        Hlen = itemsCont[count].len;
                                        Hhei = itemsCont[count].hei;
                                        Hvol = Hwid * Hlen * Hhei;
                                        holesB.push_back(Holes(Hwid,Hlen,Hhei,Hvol));
                                }
                                if ((holesF[holesId].getHei() - itemsCont[count].hei) > 0) {  //Se crea hueco Above
                                        Hwid = holesF[holesId].getWid();
                                        Hlen = holesF[holesId].getLen();
                                        Hhei = holesF[holesId].getHei() - itemsCont[count].hei;
                                        Hvol = Hwid * Hlen * Hhei;
                                        holesA.push_back(Holes(Hwid,Hlen,Hhei,Hvol));
                                }
                                if ((holesF[holesId].getLen() - itemsCont[count].len) == 0) {  //Se crea hueco Beside
                                        holesF.erase(holesF.begin() + holesId);
                                }
                                else if ((holesF[holesId].getLen() - itemsCont[count].len) > 0) {  //Se crea hueco Front
                                        Hwid = holesF[holesId].getWid();
                                        Hlen = holesF[holesId].getLen() - itemsCont[count].len;
                                        Hhei = itemsCont[count].hei;
                                        Hvol = Hwid * Hlen * Hhei;
                                        // Modificar los valores
                                        holesF.erase(holesF.begin()+holesId);   
                                        holesF.push_back(Holes(Hwid,Hlen,Hhei,Hvol)); // Modificar el hueco
                                }
                        
                        } //else if
                        holesId++;
                } //whileHolesF
        
                holesId = 0; // Se inicializa el contador de huecos
                // Se recorre segundo el vector de holesA
                while ((holesId < holesA.size()) && (label_control != true)) {
                        if ((holesA[holesId].getWid() == itemsCont[count].wid) &&
                            (holesA[holesId].getLen() == itemsCont[count].len) &&
                            (holesA[holesId].getHei() == itemsCont[count].hei)) { // si cabe justo
                                holesA.erase(holesA.begin() + holesId);  // Se elimina el hueco del vector
                                itemsCont[count].pack = true;
                                (*o2) += itemsCont[count].wei;          // Se suma el peso
                                (*o1) += itemsCont[count].vol;           // Se suma el volumen
                                label_control = true;
                        }
                        else if ((holesA[holesId].getWid() >= itemsCont[count].wid) &&
                            (holesA[holesId].getLen() >= itemsCont[count].len) &&
                            (holesA[holesId].getHei() >= itemsCont[count].hei)) { // si sobra espacio, se deben crear los nuevos huecos
                                itemsCont[count].pack = true;
                                (*o2) += itemsCont[count].wei;          // Se suma el peso
                                (*o1) += itemsCont[count].vol;           // Se suma el volumen
                                label_control = true;
                                // Calcular huecos
                                if ((holesA[holesId].getWid() - itemsCont[count].wid) > 0) {  //Se crea hueco Beside
                                        Hwid = holesA[holesId].getWid() - itemsCont[count].wid;
                                        Hlen = itemsCont[count].len;
                                        Hhei = itemsCont[count].hei;
                                        Hvol = Hwid * Hlen * Hhei;
                                        holesB.push_back(Holes(Hwid,Hlen,Hhei,Hvol));
                                }
                                if ((holesA[holesId].getLen() - itemsCont[count].len) > 0) {  //Se crea hueco Front
                                        Hwid = holesA[holesId].getWid();
                                        Hlen = holesA[holesId].getLen() - itemsCont[count].len;
                                        Hhei = itemsCont[count].hei;
                                        Hvol = Hwid * Hlen * Hhei;
                                        holesF.push_back(Holes(Hwid,Hlen,Hhei,Hvol));
                                }
                                if ((holesA[holesId].getHei() - itemsCont[count].hei) > 0) {  //Se modifica hueco Above
                                        Hwid = holesA[holesId].getWid();
                                        Hlen = holesA[holesId].getLen();
                                        Hhei = holesA[holesId].getHei() - itemsCont[count].hei;
                                        Hvol = Hwid * Hlen * Hhei;
                                        // Modificar los valores
                                        holesA.erase(holesA.begin()+holesId);   
                                        holesA.push_back(Holes(Hwid,Hlen,Hhei,Hvol)); // Modificar el hueco
                                }
                                else if ((holesA[holesId].getHei() - itemsCont[count].hei) == 0) {  //Se elimina hueco Above
                                        //cout<<"Elimina hueco above en above"<<endl;
                                        holesA.erase(holesA.begin() + holesId);
                                }
                        } //else if
                        holesId++;
                } //whileAbove
                        
                
                holesId = 0; // Se inicializa el contador de huecos
                // Se recorre tercero el vector de holesB
                while ((holesId < holesB.size()) && (label_control != true)) {
                        if ((holesB[holesId].getWid() == itemsCont[count].wid) &&
                            (holesB[holesId].getLen() == itemsCont[count].len) &&
                            (holesB[holesId].getHei() == itemsCont[count].hei)) { // si cabe justo
                                holesB.erase(holesB.begin() + holesId);  // Se elimina el hueco del vector
                                itemsCont[count].pack = true;
                                (*o2) += itemsCont[count].wei;          // Se suma el peso
                                (*o1) += itemsCont[count].vol;
                                label_control = true;
                        }
                        else if ((holesB[holesId].getWid() >= itemsCont[count].wid) &&
                            (holesB[holesId].getLen() >= itemsCont[count].len) &&
                            (holesB[holesId].getHei() >= itemsCont[count].hei)){ // si sobra espacio, se deben crear los nuevos huecos
                                itemsCont[count].pack = true;
                                (*o2) += itemsCont[count].wei;          // Se suma el peso
                                (*o1) += itemsCont[count].vol;
                                label_control = true;
                                // Calcular huecos
                                if ((holesB[holesId].getHei() - itemsCont[count].hei) > 0) {  //Se crea hueco Above
                                        Hwid = holesB[holesId].getWid();
                                        Hlen = holesB[holesId].getLen();
                                        Hhei = holesB[holesId].getHei() - itemsCont[count].hei;
                                        Hvol = Hwid * Hlen * Hhei;
                                        holesA.push_back(Holes(Hwid,Hlen,Hhei,Hvol));
                                }
                                if ((holesB[holesId].getLen() - itemsCont[count].len) > 0) {  //Se crea hueco Front
                                        Hwid = holesB[holesId].getWid();
                                        Hlen = holesB[holesId].getLen() - itemsCont[count].len;
                                        Hhei = itemsCont[count].hei;
                                        Hvol = Hwid * Hlen * Hhei;
                                        holesF.push_back(Holes(Hwid,Hlen,Hhei,Hvol));
                                }
                                if ((holesB[holesId].getWid() - itemsCont[count].wid) > 0) {  //Se modifica hueco beside
                                        Hwid = holesB[holesId].getWid() - itemsCont[count].wid;
                                        Hlen = itemsCont[count].len;
                                        Hhei = itemsCont[count].hei;
                                        Hvol = Hwid * Hlen * Hhei;
                                        // Modificar los valores
                                        holesB.erase(holesB.begin()+holesId);   
                                        holesB.push_back(Holes(Hwid,Hlen,Hhei,Hvol)); // Modificar el hueco
                                }
                                else if ((holesB[holesId].getWid() - itemsCont[count].wid) == 0) {  //Se elimina hueco Beside
                                        holesB.erase(holesB.begin() + holesId);
                                }
                        } //else if
                        holesId++;
                                        
                } //whileBeside
                
                // Parte nueva de la heuristica
                if ((itemsCont[count].wid <= W_Cont_free) && (label_control != true)){
                        LDB = itemsCont[count].wid;                    // Se establece el wid del nivel
                        W_Cont_free = W_Cont_free - LDB;             // Wid libre en el contenedor para el resto de niveles que se puedan crear
                        itemsCont[count].pack = true;
                        (*o2) += itemsCont[count].wei;                 // Se acumula el peso con la nueva pieza introducida
                        (*o1) += itemsCont[count].vol;
                
                        // Importante:
                        // Siempre que se introduce una pieza para generar un nuevo nivel,
                        // solo se producen los huecos front y above
                        
                        // Calculo de los que huecos que se generan
                        if ((lenCont - itemsCont[count].len) > 0) { // Si despues de empaquetar la pieza queda hueco en el Front
                                Hwid = LDB;
                                Hlen = lenCont - itemsCont[count].len;
                                Hhei = itemsCont[count].hei;
                                Hvol = Hwid * Hlen * Hhei;
                                holesF.push_back(Holes(Hwid,Hlen,Hhei,Hvol)); // Se crea el hueco en el vector holesF
                        }
                        if ((heiCont - itemsCont[count].hei) > 0 ) {  // Si despues de empaquetar la pieza queda hueco en el Above
                                Hwid = LDB;
                                Hlen = lenCont;
                                Hhei = heiCont - itemsCont[count].hei;
                                Hvol = Hwid * Hlen * Hhei;
                                holesA.push_back(Holes(Hwid,Hlen,Hhei,Hvol));
                        }
                }
                count++;
                holesId = 0;
                label_control = false;   // Etiqueta de control
                        
                // Ordenar los vectores de menor a mayor volumen
                sort(holesF.begin(), holesF.end(), cmpVol);
                sort(holesA.begin(), holesA.end(), cmpVol);
                sort(holesB.begin(), holesB.end(), cmpVol);
        }

        int sum = 0;
        for (unsigned int p = 0; p < itemsCont.size(); p++){
                if (itemsCont[p].pack == true){
                        sum = sum + 1;
                }
        }
        //cout<<"Cajas totales empaquetadas "<<sum<<endl;
        (*o1) = ((*o1) * 100)/volumeCont;
        holesF.clear();
        holesA.clear();
        holesB.clear();
}

// -----------------------------------------------------------------------------
// Establece la rotacion indicada por el gen para cada tipo de pieza.
// Los vectores len, wid, hei, wei y orient solo contienen los valores de los
// distintos tipos de caja (numPat)
// ------------------------------------------------------------------------------
void CLP::orientation_pieces(int id, int rot) {

        switch(rot) {
                case 0: widR = wid[id];
                        depR = len[id];
                        heiR = hei[id];
                        break;
                case 1: widR = len[id];
                        depR = wid[id];
                        heiR = hei[id];
                        break;
                case 2: widR = hei[id];
                        depR = len[id];
                        heiR = wid[id];
                        break;
                case 3: widR = len[id];
                        depR = hei[id];
                        heiR = wid[id];
                        break;
                case 4: widR = wid[id];
                        depR = hei[id];
                        heiR = len[id];
                        break;
                case 5: widR = hei[id];
                        depR = wid[id];
                        heiR = len[id];
                        break;
        } // switch
}

