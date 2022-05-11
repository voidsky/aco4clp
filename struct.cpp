#include "struct.h"
#include <random>
#include <iostream>

int uniformRandom(int maxValue) {
        return (int) (rand() / ((RAND_MAX + 1.0) / maxValue));
}

int uniformDistribution(int a, int b) {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(1, 6);
    return distrib(gen);

}

/* generate a random floating point number from min to max */
double randfrom(double min, double max) 
{
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
}
