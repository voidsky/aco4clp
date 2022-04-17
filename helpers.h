#include <iostream>
using namespace std;
class TermColor {
	public:
	void Set(int color);
	void Reset();
};

class Timer {
	public:
	string s;
	time_t start;
	void Start();
	void StopAndPrint();
	void SetS(string str);
};
