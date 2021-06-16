#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
//#include "StudentWorld.h"
//Students: Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;
class Projectile;
class Bacteria;

const double PI = 4 * atan(1);

class Actor : public GraphObject {
public:
	Actor(StudentWorld* stuWorldPtr, int imageID, double startX, double startY, Direction dir, int depth, double size)
		: GraphObject(imageID, startX, startY, dir, depth, size) {
		m_isDead = false;
		m_swPtr = stuWorldPtr;
	}
	virtual ~Actor() {}
	bool isDead() const {
		return m_isDead;
	}
	virtual void doSomething() = 0;
	void die() {
		m_isDead = true;
	}
	StudentWorld* getSWPtr() const {
		return m_swPtr;
	}
	bool overlapsWith(Actor* other) const {
		return (sqrt(pow(getX() - other->getX(), 2) + pow(getY() - other->getY(), 2)) <= SPRITE_WIDTH);
	}
	virtual bool getShot(int damage) {
		return false;
	}
	virtual bool isFood() { return false; }
	virtual bool isDirt() { return false; }
private:
	StudentWorld* m_swPtr;
	bool m_isDead;
};

class LivingActor : public Actor {
public:
	LivingActor(StudentWorld* stuWorldPtr, int hp, int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0)
		: Actor(stuWorldPtr, imageID, startX, startY, dir, depth, size) {
		m_hp = hp;
	} 
	virtual ~LivingActor() {}
	int getHealth() const { return m_hp;  }
	void setHealth(int hp) { m_hp = hp; }
private:
	int m_hp;
};

class Socrates : public LivingActor {
public:
	Socrates(StudentWorld* stuWorldPtr) : LivingActor(stuWorldPtr, 100, IID_PLAYER, 0.0, 128.0) {
		m_flameCharges = 5;
		m_sprays = 20;
	}
	virtual ~Socrates() {}
	void addFlameCharges(int numCharges) {
		m_flameCharges += numCharges;
	}
	int getFlames() { return m_flameCharges; }
	int getSprays() { return m_sprays;  }
	virtual void doSomething();
	void takeDamage(int damage);
private:
	int m_flameCharges;
	int m_sprays;
};

class Projectile : public Actor {
public:
	Projectile(int maxDist, int damage, Direction dir, int imageID, double startX, double startY, StudentWorld* stuWorldPtr)
		: Actor(stuWorldPtr, imageID, startX, startY, dir, 1, 1.0) {
		m_damage = damage;
		m_distToGo = maxDist;
	}
	virtual ~Projectile() {}
	virtual void doSomething();
	int getDamage() const { return m_damage; }
private:
	int m_damage;
	int m_distToGo;
};

class Spray : public Projectile {
public:
	Spray(double startX, double startY, Direction dir, StudentWorld* stuWorldPtr)
		: Projectile(112, 2, dir, IID_SPRAY, startX, startY, stuWorldPtr) {}
	virtual ~Spray() {}
private:
};

class Flame : public Projectile {
public:
	Flame(double startX, double startY, Direction dir, StudentWorld* stuWorldPtr)
		: Projectile(32, 5, dir, IID_FLAME, startX, startY, stuWorldPtr) {}
	virtual ~Flame() {}
private:
};

class Bacteria : public LivingActor {
public:
	Bacteria::Bacteria(int range, int damage, int hurtSound, int deathSound, StudentWorld* stuWorldPtr, int hp, int imageID, double startX, double startY)
		: LivingActor(stuWorldPtr, hp, imageID, startX, startY, 90, 0, 1.0) {
		m_hurtSound = hurtSound;
		m_deathSound = deathSound;
		m_damage = damage;
		m_range = range;
		m_munchCount = 0;
		m_movementPlanDistance = 0;
	}
	virtual ~Bacteria() {}
	virtual void doSomething();
	virtual bool getShot(int damage);
	virtual Bacteria* makebact(double x, double y) = 0;
	bool tryMove(bool isEcoli = false);
	virtual bool isEcoli() { return false; }
	bool seesSocrates(int range);
private:
	int m_range;
	int m_damage;
	int m_hurtSound;
	int m_deathSound;
	int m_munchCount;
	int m_movementPlanDistance;

	void randomizeDir() {
		int r = randInt(0, 359);
		setDirection(r);
		m_movementPlanDistance = 10;
	}
};

class DirtPile : public Actor {
public:
	DirtPile(StudentWorld* stuWorldPtr, double startX, double startY)
		: Actor(stuWorldPtr, IID_DIRT, startX, startY, 0, 1, 1.0) {

	}
	virtual ~DirtPile() {}
	virtual bool isDirt() {
		return true;
	}
	virtual bool getShot(int damage) {	
		die();
		return true;
	}
	virtual void DirtPile::doSomething() {
		return;
	}
};

class Goodie : public Actor {
public:
	Goodie(int pointVal, StudentWorld* stuWorldPtr, int imageID, double startX, double startY);
	virtual ~Goodie() {}
	void doSomething();
	virtual void doSpecifics() = 0;
private:
	int m_ticksRemaining;
	int m_pointVal;
};

class RestoreHealthGoodie : public Goodie {
public:
	RestoreHealthGoodie(double startX, double startY, StudentWorld* stuWorldPtr)
		: Goodie(250, stuWorldPtr, IID_RESTORE_HEALTH_GOODIE, startX, startY) {

	}
	virtual ~RestoreHealthGoodie() {}
	void doSpecifics();
};

class FlamethrowerGoodie : public Goodie {
public:
	FlamethrowerGoodie(double startX, double startY, StudentWorld* stuWorldPtr)
		: Goodie(300, stuWorldPtr, IID_FLAME_THROWER_GOODIE, startX, startY) {

	}
	virtual ~FlamethrowerGoodie() {}
	void doSpecifics();
};

class ExtraLifeGoodie : public Goodie {
public:
	ExtraLifeGoodie(double startX, double startY, StudentWorld* stuWorldPtr)
		: Goodie(500, stuWorldPtr, IID_EXTRA_LIFE_GOODIE, startX, startY) {

	}
	virtual ~ExtraLifeGoodie() {}
	void doSpecifics();
};

class Fungus : public Goodie {
public:
	Fungus(double startX, double startY, StudentWorld* stuWorldPtr)
		: Goodie(-50, stuWorldPtr, IID_FUNGUS, startX, startY) {

	}
	virtual ~Fungus() {}
	void doSpecifics();
};

class Food : public Actor {
public:
	Food(double startX, double startY, StudentWorld* stuWorldPtr)
		: Actor(stuWorldPtr, IID_FOOD, startX, startY, 90, 1, 1.0) {

	}
	virtual ~Food() {}
	virtual bool isFood() { return true; }
	virtual void doSomething() {
		return;
	}
private:
};

class EColi : public Bacteria {
public:
	EColi(StudentWorld* stuWorldPtr, double startX, double startY) :
		Bacteria(256, 4, SOUND_ECOLI_HURT, SOUND_ECOLI_DIE, stuWorldPtr, 5, IID_ECOLI, startX, startY) {
	}
	virtual Bacteria* makebact(double x, double y) {
		return new EColi(getSWPtr(), x, y);
	}
	virtual bool isEcoli() { return true; }
	virtual ~EColi() {}
};

class Salmonella : public Bacteria {
public:
	Salmonella(int range, int damage, int hp, StudentWorld* stuWorldPtr, double startX, double startY) :
		Bacteria(range, damage, SOUND_SALMONELLA_HURT, SOUND_SALMONELLA_DIE, stuWorldPtr, hp, IID_SALMONELLA, startX, startY) {
	}
	virtual ~Salmonella() {}
};

class RegularSalmonella : public Salmonella {
public:
	RegularSalmonella(StudentWorld* stuWorldPtr, double startX, double startY) :
		Salmonella(-1, 1, 4, stuWorldPtr, startX, startY) {
	}
	virtual ~RegularSalmonella() {}
	virtual Bacteria* makebact(double x, double y) {
		return new RegularSalmonella(getSWPtr(), x, y);
	}
};

class AggressiveSalmonella : public Salmonella {
public:
	AggressiveSalmonella(StudentWorld* stuWorldPtr, double startX, double startY) :
		Salmonella(72, 2, 10, stuWorldPtr, startX, startY) {
	}
	virtual ~AggressiveSalmonella() {}
	virtual Bacteria* makebact(double x, double y) {
		return new AggressiveSalmonella(getSWPtr(), x, y);
	}
};

class Pit : public Actor {
public:
	Pit(double startX, double startY, StudentWorld* stuWorldPtr)
		: Actor(stuWorldPtr, IID_PIT, startX, startY, 0, 1, 1.0) {
		m_nRegSalm = 5;
		m_nAggSalm = 3;
		m_nEColi = 2;
	}
	bool isempty() {
		return ((m_nRegSalm + m_nAggSalm + m_nEColi) <= 0);
	}
	virtual void doSomething();

private:
	int m_nRegSalm;
	int m_nAggSalm;
	int m_nEColi;
};

#endif // ACTOR_H_
