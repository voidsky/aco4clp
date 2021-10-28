#include "struct.h"

int uniformRandom(int maxValue) {
        return (int) (rand() / ((RAND_MAX + 1.0) / maxValue));
}