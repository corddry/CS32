#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include "Actor.h"
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
	m_socptr = nullptr;
}

StudentWorld::~StudentWorld() {
	cleanUp();
}

int StudentWorld::init()
{
	m_socptr = new Socrates(this);
	
	int rX, rY;
	for (int i = 0; i < max(180 - (20 * getLevel()), 20); ) {
		rX = randInt(0, VIEW_WIDTH);
		rY = randInt(0, VIEW_HEIGHT);
		if (sqrt(pow(rX - (VIEW_WIDTH / 2), 2) + pow(rY - (VIEW_HEIGHT / 2), 2)) <= 120) {
			m_actors.push_back(new DirtPile(this, rX, rY));
			i++;
		}
	}

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	m_socptr->doSomething();
	for (int i = 0; i < m_actors.size(); i++) {
		m_actors[0]->doSomething();
	}
	if (m_socptr->isDead()) {
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	for (int i = 0; i < m_actors.size(); i++) {
		if (m_actors[i]->isDead()) {
			delete m_actors[i];
			i = 0;
		}
	}
	return GWSTATUS_CONTINUE_GAME;
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
