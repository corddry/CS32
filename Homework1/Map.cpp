#include "Map.h"


Map::Map() : m_size(0){
}

bool Map::empty() const {
	return (m_size == 0);
}

int Map::size() const {
	return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value) {
	if (contains(key) || m_size >= DEFAULT_MAX_ITEMS) {
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
				m_itemArray[n] = m_itemArray[n+1];
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
	Item tempi;
	for (int i = 0; i < DEFAULT_MAX_ITEMS; i++) {
		tempi = m_itemArray[i];
		m_itemArray[i] = other.m_itemArray[i];
		other.m_itemArray[i] = tempi;
	}
	int tempn = m_size;
	m_size = other.m_size;
	other.m_size = tempn;
}

//S is key D is value