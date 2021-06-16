#include "provided.h"
#include <string>
#include <vector>
#include <functional>
#include "ExpandableHashMap.h"
#include <iostream>
#include <fstream>
using namespace std;

/*template<typename KeyType>
unsigned int hasher(const KeyType& k)
{
	std::hash<KeyType> h;
	return h(k);
}*/
unsigned int hasher(const GeoCoord& g)
{
    return std::hash<std::string>()(g.latitudeText + g.longitudeText);
}
unsigned int hasher(const string& s)
{
	return std::hash<std::string>()(s);
}



class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;

private:
	ExpandableHashMap<GeoCoord, vector<StreetSegment>> m_map;
	void addseg(GeoCoord start, GeoCoord end, string name);
};

StreetMapImpl::StreetMapImpl()
{
}

StreetMapImpl::~StreetMapImpl()
{
}

void StreetMapImpl::addseg(GeoCoord start, GeoCoord end, string name) {
	StreetSegment addme(start, end, name);
	vector<StreetSegment>* segs = m_map.find(start);
	if (segs == nullptr) {
		vector<StreetSegment> nuVec;	//possible scope issue might have to change to pointer and add destructorz
		nuVec.push_back(addme);
		m_map.associate(start, nuVec);
	}
	else {
		segs->push_back(addme);
	}
}

bool StreetMapImpl::load(string mapFile)
{
	ifstream input;
	input.open(mapFile);
	if (!input) {
		return false;
	}
	string line;
	while (getline(input, line))
	{
		string name = line;
		getline(input, line);
		int numSegs = stoi(line);
		for (int i = 0; i < numSegs; i++) {
			string x1, y1, x2, y2; //Temp
			input >> x1;
			input >> y1;
			input >> x2;
			input >> y2;
			GeoCoord start(x1, y1), end(x2, y2);
			addseg(start, end, name);
			addseg(end, start, name);
			//cout << "Name: " << name << " Start: " << x1 << ", " << y1 << " End: " << x2 << ", " << y2 << endl;
		}
		getline(input, line);
	} 
	return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
	const vector<StreetSegment>* found = m_map.find(gc);
	if (found == nullptr) {
		return false;
	}
	segs = *found;
	return true;
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
   return m_impl->getSegmentsThatStartWith(gc, segs);
}
