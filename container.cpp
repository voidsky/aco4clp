#include "container.h"


void Container::AddNode(Node n) 
{
        int id, rot;
        id = n.typeId;      // Identificador de la pieza
        rot = n.orientation;  // Tipo de rotacion
        double w,d,h;
        orientation_pieces(id, rot, w,d,h);

        P_Order p;
        p.wid = w;
        p.len = d;
        p.hei = h;
        p.wei = n.weight;
        p.vol = p.wid * p.hei * p.len;
        p.pack = false;
        p.index = itemsCont.size();
        itemsCont.push_back(p);
}

void Container::RemoveLast() 
{
        itemsCont.pop_back();
}

bool Container::AllPacked()
{
        for (P_Order p : itemsCont) 
                if (!p.pack) return false;
        return true;
        
}

//-------------------------------------------------------------------------------
// Evaluate
// ------------------------------------------------------------------------------
bool Container::evaluateIndividual2 (Node n, double&  o1, double& o2) {

        // Variables para controlar las dimensiones de los huecos
        double Hwid;
        double Hlen;
        double Hhei;
        double Hvol;

        double LDB;              // LDB es el ancho del nivel
        unsigned int count = 0;       // Indice dentro del vector itemsCont
        unsigned int holesId = 0;         // Indice dentro de los vectores de huecos
        bool label_control = false;

        AddNode(n);
        count = itemsCont.back().index;
 
        if ( ! (((o2) + itemsCont[count].wei) <= weight) && (((o1) + itemsCont[count].vol) <= volume) ) 
        {
                RemoveLast();
                return false;
        }
  
                holesId = 0;
                label_control = false;   // Etiqueta de control

                // Se recorre primero el vector de holesF
                while ((holesId < holesF.size()) && (label_control != true)) {
                        if ((holesF[holesId].getWid() == itemsCont[count].wid) &&
                            (holesF[holesId].getLen() == itemsCont[count].len) &&
                            (holesF[holesId].getHei() == itemsCont[count].hei)) { // si cabe justo
                                holesF.erase(holesF.begin() + holesId);  // Se elimina el hueco del vector
                                itemsCont[count].pack = true;
                                (o2) += itemsCont[count].wei;          // Se suma el peso
                                (o1) += itemsCont[count].vol;         // Se suma el volumen
                                label_control = true;
                        }
                        else if ((holesF[holesId].getWid() >= itemsCont[count].wid) &&
                            (holesF[holesId].getLen() >= itemsCont[count].len) &&
                            (holesF[holesId].getHei() >= itemsCont[count].hei)) { // si sobra espacio, se deben crear los nuevos huecos
                                itemsCont[count].pack = true;
                                (o2) += itemsCont[count].wei;          // Se suma el peso
                                (o1) += itemsCont[count].vol;         // Se suma el volumen
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
                                (o2) += itemsCont[count].wei;          // Se suma el peso
                                (o1) += itemsCont[count].vol;           // Se suma el volumen
                                label_control = true;
                        }
                        else if ((holesA[holesId].getWid() >= itemsCont[count].wid) &&
                            (holesA[holesId].getLen() >= itemsCont[count].len) &&
                            (holesA[holesId].getHei() >= itemsCont[count].hei)) { // si sobra espacio, se deben crear los nuevos huecos
                                itemsCont[count].pack = true;
                                (o2) += itemsCont[count].wei;          // Se suma el peso
                                (o1) += itemsCont[count].vol;           // Se suma el volumen
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
                                (o2) += itemsCont[count].wei;          // Se suma el peso
                                (o1) += itemsCont[count].vol;
                                label_control = true;
                        }
                        else if ((holesB[holesId].getWid() >= itemsCont[count].wid) &&
                            (holesB[holesId].getLen() >= itemsCont[count].len) &&
                            (holesB[holesId].getHei() >= itemsCont[count].hei)){ // si sobra espacio, se deben crear los nuevos huecos
                                itemsCont[count].pack = true;
                                (o2) += itemsCont[count].wei;          // Se suma el peso
                                (o1) += itemsCont[count].vol;
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
                if ((itemsCont[count].wid <= mW_Cont_free) && (label_control != true)){
                        LDB = itemsCont[count].wid;                    // Se establece el wid del nivel
                        mW_Cont_free = mW_Cont_free - LDB;             // Wid libre en el contenedor para el resto de niveles que se puedan crear
                        itemsCont[count].pack = true;
                        (o2) += itemsCont[count].wei;                 // Se acumula el peso con la nueva pieza introducida
                        (o1) += itemsCont[count].vol;
                
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
                
                // Ordenar los vectores de menor a mayor volumen
                sort(holesF.begin(), holesF.end(), cmpVol);
                sort(holesA.begin(), holesA.end(), cmpVol);
                sort(holesB.begin(), holesB.end(), cmpVol);
        
        if (itemsCont[count].pack == false) {
                //double v = itemsCont[count].vol;
                //cout << "candidate vol " << v << " container vol " << volume << " added candidate to path " << (o1)+v << " mcf " << mW_Cont_free << endl;
                RemoveLast();
                return false;
        }
         return true;

}

void Container::orientation_pieces(int id, int rot, double& widR,double& depR,double& heiR) {

        switch(rot) {
                case 0: widR = mData.wid[id];
                        depR = mData.len[id];
                        heiR = mData.hei[id];
                        break;
                case 1: widR = mData.len[id];
                        depR = mData.wid[id];
                        heiR = mData.hei[id];
                        break;
                case 2: widR = mData.hei[id];
                        depR = mData.len[id];
                        heiR = mData.wid[id];
                        break;
                case 3: widR = mData.len[id];
                        depR = mData.hei[id];
                        heiR = mData.wid[id];
                        break;
                case 4: widR = mData.wid[id];
                        depR = mData.hei[id];
                        heiR = mData.len[id];
                        break;
                case 5: widR = mData.hei[id];
                        depR = mData.wid[id];
                        heiR = mData.len[id];
                        break;
        } // switch
}

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