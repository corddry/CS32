#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

	void Socrates::doSomething() {
		if (isDead()) {
			return;
		}

		int key;
		if (getSWPtr()->getKey(key)) {
			switch (key) {
			case KEY_PRESS_SPACE:
				if (m_sprays > 0) {
					double r = (getDirection() * 1.0 + 180) / 360 * 2 * PI;
					Spray* s = new Spray(getX() - SPRITE_WIDTH * cos(r), getY() - SPRITE_WIDTH * sin(r), getDirection(), getSWPtr());
					getSWPtr()->addActor(s);
					getSWPtr()->playSound(SOUND_PLAYER_SPRAY);
					m_sprays--;
				}
				break;
			case KEY_PRESS_ENTER:
				if (m_flameCharges > 0) {
					Direction d = getDirection();
					for (int i = 0; i < 16; i++) {
						double r = ((d * 1.0) / 360) * 2 * PI;
						Flame* f = new Flame(getX() + SPRITE_WIDTH * cos(r), getY() + SPRITE_WIDTH * sin(r), d, getSWPtr());
						getSWPtr()->addActor(f);
						d = ((d + 22) % 360);
					}
					getSWPtr()->playSound(SOUND_PLAYER_FIRE);
					m_flameCharges--;
				}
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
		else if (m_sprays < 20) {
			m_sprays++;
		}
	}

	void Socrates::takeDamage(int damage) {
		setHealth(getHealth() - damage);
		if (getHealth() <= 0) {
			getSWPtr()->playSound(SOUND_PLAYER_DIE);
			die();
		}
		else {
			getSWPtr()->playSound(SOUND_PLAYER_HURT);
		}
	}

	Goodie::Goodie(int pointVal, StudentWorld* stuWorldPtr, int imageID, double startX, double startY)
		: Actor(stuWorldPtr, imageID, startX, startY, 0, 1, 1.0) {
		m_ticksRemaining = std::max(randInt(0, 300 - 10 * getSWPtr()->getLevel() - 1), 50);
		m_pointVal = pointVal;
	}

	void Goodie::doSomething() {
		m_ticksRemaining--;
		if (isDead()) {
			return;
		}
		if (overlapsWith(getSWPtr()->getsocptr())) {
			getSWPtr()->increaseScore(m_pointVal);
			doSpecifics();
			die();
			return;
		}
		if (m_ticksRemaining <= 0) {
			die();
		}
	}


	void RestoreHealthGoodie::doSpecifics() {
		getSWPtr()->getsocptr()->setHealth(100);
		getSWPtr()->playSound(SOUND_GOT_GOODIE);
	}

	void FlamethrowerGoodie::doSpecifics() {
		getSWPtr()->getsocptr()->addFlameCharges(5);
		getSWPtr()->playSound(SOUND_GOT_GOODIE);
	}

	void ExtraLifeGoodie::doSpecifics() {
		getSWPtr()->incLives();
		getSWPtr()->playSound(SOUND_GOT_GOODIE);
	}

	void Fungus::doSpecifics() {
		getSWPtr()->getsocptr()->setHealth(getSWPtr()->getsocptr()->getHealth() - 20);
	}

	void Projectile::doSomething() {
		if (isDead()) {
			return;
		}
		if (getSWPtr()->tryHit(this, true, false, getDamage())) {
			die();
			return;
		}
		moveForward(SPRITE_WIDTH);
//		if (getSWPtr()->tryHit(this)) {
//			die();							Behaves like sample if uncommented
//			return;							Behaves like spec if commented
//		}
		m_distToGo -= SPRITE_WIDTH;
		if (m_distToGo <= 0) {
			die();
		}
	}

	void Pit::doSomething() {
		if (isempty()) {
			die();
			getSWPtr()->decEvil();
		}
		int r = randInt(1, 50);
		if (r == 1 && !isempty()) {
			while (true) {
				int r2 = randInt(1, 3);
				if (r2 == 1 && m_nRegSalm > 0) {
					getSWPtr()->addActor(new RegularSalmonella(getSWPtr(), getX(), getY()));
					m_nRegSalm--;
				}
				else if (r2 == 2 && m_nAggSalm > 0) {
					getSWPtr()->addActor(new AggressiveSalmonella(getSWPtr(), getX(), getY()));
					m_nAggSalm--;
				} else if (r2 == 3 && m_nEColi > 0) {
					getSWPtr()->addActor(new EColi(getSWPtr(), getX(), getY()));
					m_nEColi--;
				} else {
					continue;
				}
				getSWPtr()->playSound(SOUND_BACTERIUM_BORN);
				getSWPtr()->incrementEvil();
				break;
			}
		}
	}

	bool Bacteria::getShot(int damage) {
		setHealth(getHealth() - damage);
		if (getHealth() <= 0 && !isDead()) {
			getSWPtr()->playSound(m_deathSound);
				getSWPtr()->decEvil();
			die();
			getSWPtr()->increaseScore(100);
			int r = randInt(1, 2);
			if (r == 1) {
				getSWPtr()->addActor(new Food(getX(), getY(), getSWPtr()));
			}
		}
		else {
			getSWPtr()->playSound(m_hurtSound);
		}
		return true;
	}

	bool Bacteria::tryMove(bool isEcoli) {
		int distance = 3;
		if (isEcoli) {
			distance = 2;
		}
		double testX = getX(), testY = getY();
		getPositionInThisDirection(getDirection(), distance, testX, testY);
		Bacteria* probe = makebact(testX, testY);
		bool hitDirt = getSWPtr()->tryHit(probe, false, true);
		delete probe;
		if (sqrt(pow(testX - (VIEW_WIDTH / 2), 2) + pow(testY - (VIEW_HEIGHT / 2), 2)) < VIEW_RADIUS
			&& !hitDirt) {
			moveTo(testX, testY);
			return true;
		}
		return false;

	}

	void Bacteria::doSomething() 
	{
		if (isDead()) {
			return;
		}
		bool isHunting = seesSocrates(m_range), hitSoc = overlapsWith(getSWPtr()->getsocptr());
		if (isHunting && !isEcoli()) {
			tryMove();
		}
		if (hitSoc) {
			getSWPtr()->getsocptr()->takeDamage(m_damage);
		}
		if ((!hitSoc || isHunting) && m_munchCount == 3) {
			double newX = getX(), newY = getY();
			if (newX < VIEW_WIDTH / 2) {
				newX += SPRITE_WIDTH / 2;
			}
			if (newX > VIEW_WIDTH / 2) {
				newX -= SPRITE_WIDTH / 2;
			}
			if (newY < VIEW_WIDTH / 2) {
				newX += SPRITE_WIDTH / 2;
			}
			if (newY > VIEW_WIDTH / 2) {
				newX -= SPRITE_WIDTH / 2;
			}
			getSWPtr()->addActor(makebact(newX, newY));
			getSWPtr()->incrementEvil();
			m_munchCount = 0;
		}
		if((!hitSoc && m_munchCount != 3) || isHunting) {
			if (getSWPtr()->tryHit(this, false, false)) {
				m_munchCount++;
			}
		}
		if (isHunting && !isEcoli()) {
			return;
		}
		if (!isEcoli()) {
			if (m_movementPlanDistance > 0) {
				m_movementPlanDistance--;
				if (!tryMove()) {
					randomizeDir();
					return;
				}
			}
			else {
				Direction findOutput = getSWPtr()->findNearestFood(this);
				if (findOutput == -1) {
					randomizeDir();
					return;
				}
				else {
					setDirection(findOutput);
					if (!tryMove()) {
						randomizeDir();
						return;
					}
				}
			}
		}
		else {
			if (isHunting) {
				for (int i = 0; i < 10; i++) {
					if (tryMove(true)) {
						break;
					}
					else {
						setDirection(getDirection() + 10);
					}
				}
			}
		}
	}

	bool Bacteria::seesSocrates(int range) {
		Socrates* soc = getSWPtr()->getsocptr();
		double dX = soc->getX() - getX();
		double dY = soc->getY() - getY();
		int d = sqrt(pow(dX, 2) + pow(dY, 2));
		if (d <= range) {
			int dir = ((atan2(dY, dX) / (2 * PI)) * 360);
			setDirection(dir);
			return true;
		}
		return false;
	}
