#include "provided.h"
#include <vector>
using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
private:
	const StreetMap* m_sm;
	PointToPointRouter m_ptp;
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm) : m_ptp(sm)
{
	m_sm = sm;
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{
}

string angletoDir(double angle) {
	if (0 <= angle && angle < 22.5)
		return "east";
	if (22.5 <= angle && angle < 67.5)
		return "northeast";
	if (67.5 <= angle && angle < 112.5)
		return "north";
	if (112.5 <= angle && angle < 157.5)
		return "northwest";
	if (157.5 <= angle && angle < 202.5)
		return "west";
	if (202.5 <= angle && angle < 247.5)
		return "southwest";
	if (247.5 <= angle && angle < 292.5)
		return "south";
	if (292.5 <= angle && angle < 337.5)
		return "southeast";
	if (angle >= 337.5)
		return "east";
	else
		return "ShouldNeverGetHere";
}

void ptptocomm(vector<DeliveryCommand>& commands, list<StreetSegment> partialRoute) {
	DeliveryCommand addme;
	list<StreetSegment>::iterator it = partialRoute.begin();

	double segdist = distanceEarthMiles(it->start, it->end);
	addme.initAsProceedCommand(angletoDir(angleOfLine(*it)), it->name, segdist);
	commands.push_back(addme);
	it++;
	for (; it != partialRoute.end(); it++) {
		segdist = distanceEarthMiles(it->start, it->end);

		if (it->name == commands.back().streetName()) {
			commands.back().increaseDistance(segdist);
			continue;
		}
		double turnangle = angleBetween2Lines(*(prev(it)), *it);

		if (turnangle < 1 || turnangle > 359) {
			addme.initAsProceedCommand(angletoDir(angleOfLine(*it)), it->name, segdist);
			commands.push_back(addme);
			continue;
		}
		else if (turnangle >= 1 && turnangle < 180) {
			addme.initAsTurnCommand("left", it->name);
		}
		else {
			addme.initAsTurnCommand("right", it->name);
		}

		commands.push_back(addme);
		addme.initAsProceedCommand(angletoDir(angleOfLine(*it)), it->name, segdist); //add Directions
		commands.push_back(addme);
	}
}



DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
	
	totalDistanceTravelled = 0.0;
	commands.clear();

	DeliveryOptimizer opto(m_sm);

	double oldcrow, newcrow;
	vector<DeliveryRequest> deliveriez;
	for (int i = 0; i < deliveries.size(); i++) {
		deliveriez.push_back(deliveries[i]);
	}

	opto.optimizeDeliveryOrder(depot, deliveriez, oldcrow, newcrow);

	GeoCoord currloc = depot;
	//list<StreetSegment> route;
	list<StreetSegment> partialRoute;
	double partialDist;
	for (int i = 0; i < deliveriez.size(); i++) {
		if (m_ptp.generatePointToPointRoute(currloc, deliveriez[i].location, partialRoute, partialDist) != DELIVERY_SUCCESS) {
			return (m_ptp.generatePointToPointRoute(currloc, deliveriez[i].location, partialRoute, partialDist));
		}

		ptptocomm(commands, partialRoute);

		DeliveryCommand addme;
		addme.initAsDeliverCommand(deliveriez[i].item);
		commands.push_back(addme);

		currloc = deliveriez[i].location;
		totalDistanceTravelled += partialDist;
	}
	m_ptp.generatePointToPointRoute(currloc, depot, partialRoute, partialDist);
	ptptocomm(commands, partialRoute);
	totalDistanceTravelled += partialDist;

    return DELIVERY_SUCCESS;
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}
