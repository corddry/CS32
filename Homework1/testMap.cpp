#include "Map.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	Map m;  // maps s to d
	assert(m.empty());
	ValueType v = -1234.5;
	assert(!m.get("abc", v) && v == -1234.5); // v unchanged by get failure
	m.insert("xyz", 9876.5);
	assert(m.size() == 1);
	KeyType k = "hello";
	assert(m.get(0, k, v) && k == "xyz" && v == 9876.5);

	m.insert("asd", 1337);
	assert(m.size() == 2);
	assert(!m.update("qwe", 111) && m.update("asd", 8));
	assert(m.get("asd", v) && v == 8);
	assert(m.size() == 2);
	assert(m.insertOrUpdate("qwe", 111) && m.update("asd", 777));
	assert(m.get("asd", v) && v == 777);
	assert(m.insert("abc", 123) && !m.insert("abc", 321));
	assert(!m.erase("eee") && m.erase("abc"));
	assert((m.size() == 3) && !m.get("abc", v) && m.get(1, k, v));
	assert(k != "abc" && v != 123);
	assert(m.contains("asd") && !m.contains("abc"));

	Map m2;
	m2.swap(m);
	assert(m2.size() == 3 && m.empty());
	assert(m2.contains("asd") && !m.contains("asd"));

	cout << "Passed all tests" << endl;

}

/*
#include "Map.h"
#include <iostream>
#include <cassert>
using namespace std;


int main()
{
	Map m;  // maps ints to ss
	assert(m.empty());
	ValueType v = "Ouch";
	assert(!m.get(42, v) && v == "Ouch"); // v unchanged by get failure
	m.insert(123456789, "Wow!");
	assert(m.size() == 1);
	KeyType k = 9876543;
	assert(m.get(0, k, v) && k == 123456789 && v == "Wow!");
	cout << "Passed all tests" << endl;
}
*/

