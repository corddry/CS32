
//Your declarationsand implementations would go here
class Event {
public:
	Event(string name) : m_name(name) {};
	virtual ~Event() {};
	string name() const { return m_name; }
	virtual string need() const = 0;
	virtual bool isSport() const = 0;
private:
	string m_name;
	string m_need;
};

class BasketballGame : public Event {
public:
	BasketballGame(string name) : Event(name) {};
	~BasketballGame() {
		cout << "Destroying the " << name() << " basketball game" << endl;
	}
private:
	virtual string need() const { return "hoops"; }
	virtual bool isSport() const { return true; }
};
class Concert : public Event {
public:
	Concert(string name, string genre) : Event(name), m_genre(genre) {};
	~Concert() {
		cout << "Destroying the " << name() << " " << m_genre << " concert" << endl;
	}
private:
	virtual string need() const { return "a stage"; }
	virtual bool isSport() const { return false; }
	string m_genre;
};
class HockeyGame : public Event {
public:
	HockeyGame(string name) : Event(name) {};
	~HockeyGame() {
		cout << "Destroying the " << name() << " hockey game" << endl;
	}
private:
	virtual string need() const { return "ice"; }
	virtual bool isSport() const { return true; }
};