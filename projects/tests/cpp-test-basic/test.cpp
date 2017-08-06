// Compile in RASPBIAN as 
// gcc test.cpp -o testcpp -lstdc++
using namespace std;
#include <iostream>
#include "types.h"

int main(int argc, char* argv[])
{
	cout << "Hello World\n";
	for (int i=0; i<argc; i++)
		cout << "arg[i]=" << argv[i] << endl;

	return 0;
}
