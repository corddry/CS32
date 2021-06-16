#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
#include "Actor.h"
using namespace std;

class Projectile;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath){
	m_points = 0;
	m_activeEvils = 0;
	m_socptr = nullptr;
}

StudentWorld::~StudentWorld() {
	cleanUp();
}

void StudentWorld::addInitActor(Actor* addme, int overlapAllowedIndex) {
	int rX, rY;
	bool clear;
	for (;;) {
		clear = true;
		rX = randInt(0, VIEW_WIDTH);
		rY = randInt(0, VIEW_HEIGHT);
		if (sqrt(pow(rX - (VIEW_WIDTH / 2), 2) + pow(rY - (VIEW_HEIGHT / 2), 2)) <= 120) {
			addme->moveTo(rX, rY);
			for (int i = 0; i < overlapAllowedIndex; i++) {
				if (addme->overlapsWith(m_actors[i])) {
					clear = false;
				}
			}
			if (clear) {
				m_actors.push_back(addme);
				return;
			}
		}
	}
}

int StudentWorld::init()
{

	m_activeEvils = 0;

	m_socptr = new Socrates(this);
	
	for (int i = 0; i < getLevel(); i++) {
		addInitActor(new Pit(0, 0, this), m_actors.size());
		incrementEvil();
	}

	for(int i = 0; i < min(5 * getLevel(), 25); i++) {
		addInitActor(new Food(0, 0, this), m_actors.size());
	}

	int numNonDirts = m_actors.size();
	for (int i = 0; i < max(180 - (20 * getLevel()), 20); i++) {
		addInitActor(new DirtPile(this, 0, 0), numNonDirts);
	}

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	for (int i = 0; i < m_actors.size(); i++) {
		m_actors[i]->doSomething();
	}
	m_socptr->doSomething();
	if (m_socptr->isDead()) {
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	for (int i = 0; i < m_actors.size(); i++) {
		if (m_actors[i]->isDead()) {
			delete m_actors[i];
			m_actors.erase(m_actors.begin() + i);
			i = 0;
		}
	}

	int chanceFungus = std::max(510 - getLevel() * 10, 200), chanceGoodie = std::max(510 - getLevel() * 10, 250);
	if (randInt(0, chanceFungus) == 0) {
		addGoodie(new Fungus(0,0, this));
	}
	if (randInt(0, chanceGoodie) == 0) {
		int r = randInt(0, 9);
		if (r < 6) {
			addGoodie(new RestoreHealthGoodie(0, 0, this));
		}
		else if (r < 9) {
			addGoodie(new FlamethrowerGoodie(0, 0, this));
		}
		else {
			addGoodie(new ExtraLifeGoodie(0, 0, this));
		}
		
	}

	ostringstream s;
	s << "Score: " << getScore() << "  Level: " << getLevel() << "  Lives: " << getLives() << "  Health: " << getsocptr()->getHealth() << "  Sprays: " << getsocptr()->getSprays() << " Flames:  " << getsocptr()->getFlames() << " ";
	setGameStatText(s.str());

	if (m_activeEvils == 0) {
		return GWSTATUS_FINISHED_LEVEL;
	}

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::addGoodie(Goodie *addme) {
	double theta = 2*PI * (double)rand() / (double)RAND_MAX;
	addme->moveTo(VIEW_WIDTH / 2 + VIEW_RADIUS * cos(theta), VIEW_WIDTH / 2 + VIEW_RADIUS * sin(theta));
	m_actors.push_back(addme);
}

bool StudentWorld::tryHit(Actor* incident, bool isProj, bool isMovementOverlap, int damage) {
	for (int i = 0; i < m_actors.size(); i++) {
		Actor* a = m_actors[i];
		if (!isMovementOverlap) {
			if (incident->overlapsWith(m_actors[i])) {
				if (isProj && a->getShot(damage)) {
					return true;
				}
				else if (!isProj && a->isFood()) {
					a->die();
					return true;
				}
			}
		}
		else if (a->isDirt() && sqrt(pow(incident->getX() - a->getX(), 2) + pow(incident->getY() - a->getY(), 2)) <= SPRITE_WIDTH / 2) {
			return true;
		}
	}
	return false;
}


void StudentWorld::addActor(Actor* addme) {
	m_actors.push_back(addme);
}

Direction StudentWorld::findNearestFood(Bacteria* sniffer) const{
	int dNearest = 129;
	Direction dir = -1;
	for (int i = 0; i < m_actors.size(); i++) {
		if (!m_actors[i]->isFood()) {
			continue;
		}
		double dX = m_actors[i]->getX() -sniffer->getX();
		double dY = m_actors[i]->getY() - sniffer->getY();
		int d = sqrt(pow(dX, 2) + pow(dY, 2));
		if (d < dNearest) {
			dir = ((atan2(dY, dX)/(2*PI)) *360);
			dNearest = d;
		}
	}
	return dir;
}


void StudentWorld::cleanUp()
{
		delete m_socptr;
		m_socptr = nullptr;
	while (m_actors.size() != 0) {
		delete m_actors[0];
		m_actors.erase(m_actors.begin());
	}
}
