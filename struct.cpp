#include "struct.h"

int uniformRandom(int maxValue) {
        return (int) (rand() / ((RAND_MAX + 1.0) / maxValue));
}

/* generate a random floating point number from min to max */
double randfrom(double min, double max) 
{
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
}
