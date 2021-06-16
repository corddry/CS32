#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
//#include "StudentWorld.h"
class StudentWorld;
//Students: Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

const double PI = 4 * atan(1);

class Actor : public GraphObject {
public:
	Actor(StudentWorld* stuWorldPtr, int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0);
	virtual bool isDead();
	virtual void doSomething() = 0;
	virtual void die();
	int getPts();
	StudentWorld* getSWPtr();
private:
	StudentWorld* m_swPtr;
	bool m_isDead;
	bool m_damageable;
	int m_pointVal;
};

class LivingActor : public Actor {
public:
	LivingActor(StudentWorld* stuWorldPtr, int hp, int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0);
private:
	int m_hp;
};

class Socrates : public LivingActor {
public:
	Socrates(StudentWorld* stuWorldPtr);
	virtual void doSomething();
};

class Bacteria : public LivingActor {
public:
	Bacteria(StudentWorld* stuWorldPtr, int hp, int imageID, double startX, double startY);
private:

};

class DirtPile : public Actor {
public:
	DirtPile(StudentWorld* stuWorldPtr, double startX, double startY);
	virtual void doSomething();
};

#endif // ACTOR_H_
