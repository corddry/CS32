#include "CarMap.h"
#include <iostream>

CarMap::CarMap() {

}

bool CarMap::addCar(std::string liscense) {
	return (m_map.insert(liscense, 0));
}

double CarMap::miles(std::string liscense) const {
	double milesReturn;
	if (!m_map.get(liscense, milesReturn)) {
		return -1;
	}
	return milesReturn;
}

bool CarMap::drive(std::string liscense, double distance) {
	if (!m_map.contains(liscense) || distance < 0) {
		return false;
	}
	double odometer;
	m_map.get(liscense, odometer);
	return( m_map.update(liscense, (odometer + distance)) );
}

int CarMap::fleetSize() const {
	return m_map.size();
}

void CarMap::print() const {
	std::string liscense;
	double distance;
	for (int i = 0; m_map.get(i, liscense, distance); i++) {
		std::cout << liscense << " " << distance << std::endl;
	}
}

