#include <stdio.h>
#include "helpers.h"
using namespace std;

void Timer::SetS(string str) {
	s = str;
}
void Timer::Start() {
	start = clock();
}

void Timer::StopAndPrint() {
	time_t t = clock() - start;

	auto t1 = ((float)t)/CLOCKS_PER_SEC;
	printf("%f \n", t1);
}
