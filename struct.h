#ifndef STRUCT_H
#define STRUCT_H

#include <stdlib.h> 
#include <time.h> 
#include <stdio.h>
#include <iostream>
#include <vector>

enum ANTOBJECTIVE { OBJ_WEIGHT, OBJ_VOLUME, OBJ_WEIGHTANDVOLUME };

typedef struct P_Order {
	int idItem;
	int pat;
	double len;
	double wid;
	double hei;
	double wei;
	double vol;
	int orient;
	bool pack;
	int number;
	int index;   // Mantener actualizados auxWei y auxWid
} P_Order;

struct Arc;

/*typedef struct P_Cont {
	int id;
	int typeId;
	int orientations;
	//double * bottomArea;
	double volume;
	double weight;
} P_Cont;
*/
/* We will represent ant path as a vector of Nodes */
typedef struct Node {
	int id;	
	int typeId;  // Piece identifier
	int orientations;
	int orientation;	// Orientation for pieces
	double volume;
	double weight;
	

  bool operator<(const Node& a) const
    {
        return volume < a.volume;
    }
  
  bool operator>(const Node& a) const
    {
        return volume > a.volume;
    }  
} Node;



/* Random from 0 to maxValue-1 */
int uniformRandom(int maxValue);
double randfrom(double min, double max);
int uniformDistribution(int a, int b);

#endif