#include "provided.h"
#include <list>
#include <set>
#include "ExpandableHashMap.h"
using namespace std;

class PointToPointRouterImpl
{
public:
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    DeliveryResult generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const;
private:
	const StreetMap* m_sm;
	struct Node {
		//GeoCoord coord;
		StreetSegment seg;
		Node* parent;
		double f, g;
		
		/*bool operator<(const Node& other) {
			return coord < other.coord;
			//return f < other.f; //Lesser Node is the closer one
		}*/
	};
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
	m_sm = sm;
}

PointToPointRouterImpl::~PointToPointRouterImpl()
{
}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
	//Learned A* from vvv
	//https://www.geeksforgeeks.org/a-search-algorithm/
	vector<Node> open;
	vector<Node*> closed;
	//ExpandableHashMap<GeoCoord, bool> closed;

	totalDistanceTravelled = 0;
	route.clear();
	Node first;
	first.parent = nullptr;
	StreetSegment starter(GeoCoord(), start, "StartingPt");
	first.seg = starter;
	first.f = distanceEarthMiles(start, end);
	first.g = 0;

	open.push_back(first);

	vector<StreetSegment> segs;
	if (!(m_sm->getSegmentsThatStartWith(start, segs) && m_sm->getSegmentsThatStartWith(end, segs))) {
		return BAD_COORD;
	}

	while (!open.empty()) {

		vector<Node>::iterator it;
		vector<Node>::iterator least = open.begin();
		double leastF = least->f;
		for (it = open.begin(); it != open.end(); it++) {
			if (it->f < leastF) {				//O(Nopen)
				least = it;
				leastF = it->f;
			}
		}
		Node curr = *least;
		Node* dyncurr = new Node;
		*dyncurr = curr;
		open.erase(least);

		m_sm->getSegmentsThatStartWith(curr.seg.end, segs);

		for (int i = 0; i < segs.size(); i++) {
		//di

			Node successor;
			successor.seg = segs[i];
			successor.parent = dyncurr;
			double g = curr.g + distanceEarthMiles(curr.seg.end, successor.seg.end);
			double h = distanceEarthMiles(successor.seg.end, end);

			successor.g = g;
			successor.f = g + h;

			if (h == 0) {
				totalDistanceTravelled = g;
				route.push_front(successor.seg);
				Node* pushme = successor.parent;
				while (pushme->parent != nullptr) { //DONT INCLUDE STARTER
					route.push_front(pushme->seg);
					pushme = pushme->parent;
					//cout << pushme->seg.name;
				}
				while (!closed.empty()) {
					delete closed.back();
					closed.pop_back();
				}
				delete dyncurr;
				return DELIVERY_SUCCESS;
			}
			
			bool skip = false;
			it = open.begin();
			for (int i = 0; i < open.size(); i++) {
				if (open[i].seg.end == successor.seg.end) {				//O(Nopen)
					if (open[i].f < successor.f) {
						skip = true;
					}
				}
			}
			for (int i = 0; i < closed.size(); i++) {
				if (closed[i]->seg.end == successor.seg.end) {				//O(Nopen)
					if (closed[i]->f < successor.f) {
						skip = true;
					}
				}
			}
			if (!skip) {
				open.push_back(successor);
			}
		}
		
		closed.push_back(dyncurr);
	}
	while (!closed.empty()) {
		delete closed.back();
		closed.pop_back();
	}
    return NO_ROUTE;
}

//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
    m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
    delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}
