#ifndef STRUCT_H
#define STRUCT_H

#include <stdlib.h> 
#include <time.h> 
#include <stdio.h>
#include <iostream>
#include <vector>

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


typedef struct P_Cont {
	int id;
	int typeId;
	int orientations;
	double* pheromones;
} P_Cont;

/* We will represent ant path as a vector of Nodes */
typedef struct Node {
	int typeId;  // Piece identifier
	int count;	// Number of pieces of id type to be used
	int orientation;	// Orientation for pieces
	P_Cont * ptrToMapNode;
} Node;



/* Random from 0 to maxValue-1 */
int uniformRandom(int maxValue);

#endif