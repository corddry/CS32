#include "provided.h"
#include <vector>
using namespace std;

class DeliveryOptimizerImpl
{
public:
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    void optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const;
private:
	const StreetMap* m_sm;
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
{
	m_sm = sm;
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{
}

vector<DeliveryRequest>::iterator findClosest(GeoCoord start, vector<DeliveryRequest>& deliveries) {
	vector<DeliveryRequest>::iterator it = deliveries.begin();
	vector<DeliveryRequest>::iterator bestNext = it;
	double least = distanceEarthMiles(start, it->location);
	it++;
	for (; it != deliveries.end(); it++) {
		if (distanceEarthMiles(start, it->location) < least) {				//O(Nopen)
			bestNext = it;
			least = distanceEarthMiles(start, it->location);
		}
	}
	return bestNext;
}

void DeliveryOptimizerImpl::optimizeDeliveryOrder(
    const GeoCoord& depot,
    vector<DeliveryRequest>& deliveries,
    double& oldCrowDistance,
    double& newCrowDistance) const
{
	oldCrowDistance = 0;

	if (!deliveries.empty()) {
		oldCrowDistance += distanceEarthMiles(depot, deliveries[0].location);
	}

	for (int i = 0; i < deliveries.size() - 1; i++) {
		oldCrowDistance += distanceEarthMiles(deliveries[i].location, deliveries[i+1].location);
	}

	if (!deliveries.empty()) {
		oldCrowDistance += distanceEarthMiles(deliveries.back().location, depot);
	}

	newCrowDistance = 0;

	vector<DeliveryRequest> optimized;
	
	GeoCoord curr = depot;
	while (!deliveries.empty()) {
		vector<DeliveryRequest>::iterator closest = findClosest(curr, deliveries);
		optimized.push_back(*closest);
		deliveries.erase(closest);
	}
	deliveries = optimized;

	if (!deliveries.empty()) {
		newCrowDistance += distanceEarthMiles(depot, deliveries[0].location);
	}

	for (int i = 0; i < deliveries.size() - 1; i++) {
		newCrowDistance += distanceEarthMiles(deliveries[i].location, deliveries[i + 1].location);
	}

	if (!deliveries.empty()) {
		newCrowDistance += distanceEarthMiles(deliveries.back().location, depot);
	}


}


//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
    m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
    delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const
{
    return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}
