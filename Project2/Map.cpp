#include "Map.h"
Map::Map() {
	head = nullptr;
	tail = nullptr;
	numNodes = 0;
}

Map::Map(const Map& src) {
	head = nullptr;
	tail = nullptr;
	numNodes = 0;
	KeyType key;
	ValueType value;
	int i = src.numNodes - 1;			//Because insert inserts at the top,
	while (src.get(i, key, value)) {	//insert the copied pairs last to first
		insert(key, value);
		i--;
	}
}

Map::~Map() {
	int i = 0;
	KeyType key;
	ValueType value;
	while (get(i, key, value)) {		//get() returns false once i is out of bounds
		erase(key);
		i++;
	}
}

Map& Map::operator=(const Map& src) {
	int i = src.numNodes - 1;
	KeyType key;
	ValueType value;
	while (get(i, key, value)) {
		erase(key);
		i--;
	}
	dump();

	i = 0;
	while (src.get(i, key, value)) {
		insert(key, value);
		i++;
	}

	return *this;
}

bool Map::empty() const {
	return (numNodes == 0);
}

int Map::size() const {
	return numNodes;
}

bool Map::insert(const KeyType& key, const ValueType& value) {
	
	Node* iterator = head;
	while (iterator != nullptr) {
		if (iterator->key == key) {
			return false;				//check that key isnt already in the map
		}
		iterator = iterator->next;
	}

	Node* newNode = new Node;
	numNodes++;

	if (head != nullptr) {
		head->prev = newNode;
	}
	newNode->next = head;
	newNode->prev = nullptr;			//prev is always nullptr because pairs are inserted at the top

	head = newNode;
	
	if (numNodes == 1) {
		tail = newNode;
	}

	newNode->key = key;
	newNode->val = value;
	
	return true;
}

bool Map::update(const KeyType& key, const ValueType& value) {
	Node* iterator = head;
	while (iterator != nullptr) {
		if (iterator->key == key) {
			iterator->val = value;
			return true;
		}
		iterator = iterator->next;
	}
	return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
	if (!insert(key, value)) {
		return update(key, value);
	}
	return true;
}
bool Map::erase(const KeyType& key) {
	Node* iterator = head;
	while (iterator != nullptr) {
		if (iterator->key == key) {
			if (iterator->prev == nullptr) {		//if the pair is the first in the map, reassign head
				head = iterator->next;
			}
			else {
				iterator->prev->next = iterator->next;		//conect previous node to next
			}
			if (iterator->next == nullptr) {		//if the pair is the last in the map
				tail = iterator->prev;
			}
			else {
				iterator->next->prev = iterator->prev;		//connect next node to previous
			}
			
			delete iterator;
			numNodes--;
			return true;
		}
		iterator = iterator->next;
	}
	return false;
}

bool Map::contains(const KeyType& key) const {
	Node* iterator = head;
	while (iterator != nullptr) {
		if (iterator->key == key) {
			return true;
		}
		iterator = iterator->next;
	}
	return false;
}

bool Map::get(const KeyType& key, ValueType& value) const {
	Node* iterator = head;
	while (iterator != nullptr) {
		if (iterator->key == key) {
			value = iterator->val;
			return true;
		}
		iterator = iterator->next;
	}
	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const {
	if (!(0 <= i && i < size())) {
		return false;
	}
	Node* iterator = head;
	if (iterator == nullptr) {
		return false;
	}
	for (int j = 0; j < i; j++) {
		if (iterator == nullptr) {
			return false;
		}
		iterator = iterator->next;
	}
	key = iterator->key;
	value = iterator->val;
	return true;
}

void Map::swap(Map& other) {
	Node* tempHead = head;
	Node* tempTail = tail;
	int tempNumNodes = numNodes;

	head = other.head;
	other.head = tempHead;
	tail = other.tail;
	other.tail = tempTail;
	numNodes = other.numNodes;
	other.numNodes = tempNumNodes;
}

bool combine(const Map& m1, const Map& m2, Map& result){
	Map resultToBe;										//start with a blank map
	
	ValueType v1, v2;
	KeyType k1, k2;

	bool ret = true;

	bool addval = true;
	for (int i = 0; i < m1.size(); i++) {
		m1.get(i, k1, v1);
		addval = true;
		for (int j = 0; j < m2.size(); j++) {
			m2.get(j, k2, v2);
			if (k1 == k2) {							//each pair in m1 gets compared with each pair in m2 and the
				if (v1 != v2) {						//bool to return is set to false if two keys are the same, but
					ret = false;					//have different values. Otherwise, if the pair in m1 has no
					addval = false;					//other-valued match, add it to the result.
				}
			}
		}
		if (addval) {
			resultToBe.insert(k1, v1);
		}
	}
	
	for (int i = 0; i < m2.size(); i++) {			//Repeat for all the pairs in m2
		addval = true;
		m2.get(i, k1, v1);
		for (int j = 0; j < m1.size(); j++) {
			m1.get(j, k2, v2);
			if (k1 == k2) {
				if (v1 != v2) {
					ret = false;
				}	
				addval = false;			//Skips adding matching pairs to the result because they've been counted for in the first loop
			}
		}
		if (addval) {
			resultToBe.insert(k1, v1);
		}
	}
	result = resultToBe;
	return ret;
}

void reassign(const Map& m, Map& result) {
	if (m.size() <= 1) {
		return;
	}
	
	Map resultToBe;

	KeyType key;
	ValueType ival, nextVal;
	int i = m.size() - 1;
	m.get(0, key, nextVal);			//Reassigns and inserts the final
	m.get(i, key, ival);			//pair on the list with the value
	result.insert(key, nextVal);	//from the first pair on the list

	for (; i >= 0; i--) {
		if (m.get(i+1, key, nextVal)) {			//gets the data of the pair after pair i
			m.get(i, key, ival);				//gets the data of pair i
			result.insert(key, nextVal);		//inserts the reassigned pair into result
		}
	}

	result = resultToBe;
}


#include <iostream>
void Map::dump() const{
	std::cerr << "head: " << head << " | tail: " << tail << std::endl;
	Node* iterator = head;
	for (int i = 0; i < numNodes; i++) {
		std::cerr << "i: " << i << " | iterator: " << iterator << " | iterator->key: " << iterator->key << " | iterator->val: " 
			<< iterator->val << " | iterator->next: " << iterator->next << " | iterator->prev: " << iterator->prev << std::endl;
		iterator = iterator->next;
	}
}
