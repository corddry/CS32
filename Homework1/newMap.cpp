#include "newMap.h"


Map::Map() : m_size(0), m_maxPairs(DEFAULT_MAX_ITEMS){
	m_itemArray = new Item[m_maxPairs];
}

Map::Map(int maxPairs) : m_size(0), m_maxPairs(maxPairs){
	m_itemArray = new Item[m_maxPairs];
}

Map::Map(const Map& src) {
	m_itemArray = new Item[0];
	*this = src;
}

Map& Map::operator=(const Map& src) {
	delete[] m_itemArray;
	m_size = src.m_size;
	m_maxPairs = src.m_maxPairs;
	m_itemArray = new Item[m_maxPairs];
	for (int i = 0; i < m_maxPairs; i++) {
		m_itemArray[i].key = src.m_itemArray[i].key;
		m_itemArray[i].value = src.m_itemArray[i].value;
	}
	return *this;
}

Map::~Map() {
	delete[] m_itemArray;
}

bool Map::empty() const {
	return (m_size == 0);
}

int Map::size() const {
	return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value) {
	if (contains(key) || m_size >= m_maxPairs) {
		return false;
	}
	m_itemArray[m_size].key = key;
	m_itemArray[m_size].value = value;

	m_size++;
	return true;
}
bool Map::update(const KeyType& key, const ValueType& value) {
	for (int i = 0; i < m_size; i++) {
		if (m_itemArray[i].key == key) {
			m_itemArray[i].value = value;
			return true;
		}
	}
	return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
	if (update(key, value)) {
		return true;
	}
	else {
		return(insert(key, value));
	}
}

bool Map::erase(const KeyType& key) {
	for (int i = 0; i < m_size; i++) {
		if (m_itemArray[i].key == key) {
			for (int n = i; n < m_size - 1; n++) {
				m_itemArray[n] = m_itemArray[n + 1];
			}
			m_size--;
			return true;
		}
	}
	return false;
}

bool Map::contains(const KeyType& key) const {
	for (int i = 0; i < m_size; i++) {
		if (m_itemArray[i].key == key) {
			return true;
		}
	}
	return false;
}

bool Map::get(const KeyType& key, ValueType& value) const {
	for (int i = 0; i < m_size; i++) {
		if (m_itemArray[i].key == key) {
			value = m_itemArray[i].value;
			return true;
		}
	}
	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const {
	if (0 <= i && i < m_size) {
		key = m_itemArray[i].key;
		value = m_itemArray[i].value;
		return true;
	}
	return false;
}

void Map::swap(Map& other) {
	Map tempm;
	tempm = *this;
	*this = other;
	other = tempm;
}
