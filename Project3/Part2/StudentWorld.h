#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>
#include "GraphObject.h"

class Actor;
class Socrates;
class Goodie;
class Projectile;
class Bacteria;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
	virtual ~StudentWorld();
	Socrates* getsocptr() {
		return m_socptr;
	}
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	bool tryHit(Actor* incident, bool isProj, bool isMovementOverlap, int damage = 0);
	void addActor(Actor* addme);
	void incrementEvil() {
		m_activeEvils++;
	}
	void decEvil() { m_activeEvils--;
	}
	Direction findNearestFood(Bacteria* sniffer) const;
private:
	std::vector<Actor*> m_actors;
	Socrates* m_socptr;
	int m_points;
	int m_activeEvils;
	void addGoodie(Goodie* addme);
	void addInitActor(Actor* addme, int overlapAllowedIndex);
};

#endif // STUDENTWORLD_H_
