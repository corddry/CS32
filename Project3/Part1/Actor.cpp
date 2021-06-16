#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(StudentWorld* stuWorldPtr, int imageID, double startX, double startY, Direction dir, int depth, double size)
		: GraphObject(imageID, startX, startY, dir, depth, size) {
		m_isDead = false;
		m_damageable = true;
		m_pointVal = 0;		//REDEFINE FOR SUBCLASSES
		m_swPtr = stuWorldPtr;
	}
	bool Actor::isDead() {
		return m_isDead;
	}
	void Actor::die() {
		m_isDead = true;
	}
	int Actor::getPts() {
		return m_pointVal;
	}
	StudentWorld* Actor::getSWPtr() {
		return m_swPtr;
	}

	LivingActor::LivingActor(StudentWorld* stuWorldPtr, int hp, int imageID, double startX, double startY, Direction dir, int depth, double size)
		: Actor(stuWorldPtr, imageID, startX, startY, dir, depth, size) {
		m_hp = hp;
	} 


	Socrates::Socrates(StudentWorld* stuWorldPtr) : LivingActor(stuWorldPtr, 100, IID_PLAYER, 0, 128) {
		
	}

	void Socrates::doSomething() {
		if (isDead()) {
			return;
		}

		int key;
		if (getSWPtr()->getKey(key)) {
			switch (key) {
			case KEY_PRESS_SPACE:
				//spray();
				break;
			case KEY_PRESS_ENTER:
				//flame();
				break;
			case KEY_PRESS_LEFT:
				setDirection(getDirection() + 10);
			case KEY_PRESS_RIGHT:
				setDirection(getDirection() - 5);
				double r = (getDirection() * 1.0 + 180) / 360 * 2 * PI;
				moveTo(cos(r) * 128 + 128, sin(r) * 128 + 128);
				break;
			}
		}
		
	}

	DirtPile::DirtPile(StudentWorld* stuWorldPtr, double startX, double startY)
		: Actor(stuWorldPtr, IID_DIRT, startX, startY, 0, 1, 1.0){

	}
	void DirtPile::doSomething() {
		return;
	}

	Bacteria::Bacteria(StudentWorld* stuWorldPtr, int hp, int imageID, double startX, double startY)
		: LivingActor(stuWorldPtr, hp, imageID, startX, startY, 90, 0, 1.0) {

	}