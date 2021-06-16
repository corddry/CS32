#include "CarMap.h"
#include <iostream>
#include <cassert>

using namespace std;

int main() {
	CarMap cm;
	cout << "print #1:" << endl;
	cm.print();
	cout << endl;
	assert(cm.fleetSize() == 0);
	assert(cm.addCar("asd"));
	assert(!cm.addCar("asd"));
	assert(cm.miles("asd") == 0 && cm.miles("sdf") == -1);
	assert(cm.fleetSize() == 1);
	cout << "print #2:" << endl;
	cm.print();
	cout << endl;
	assert(cm.addCar("qwerty"));
	assert(cm.fleetSize() == 2);
	assert(cm.drive("asd", 1337) && !cm.drive("asd", -100));
	assert(cm.miles("asd") == 1337 && cm.miles("qwerty") == 0);
	cout << "print #3:" << endl;
	cm.print();
	cout << endl;

	cout << "all tests passed" << endl;

}
