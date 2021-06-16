#ifndef EXPANDABLEHASHMAP
#define EXPANDABLEHASHMAP

// ExpandableHashMap.h

#include "provided.h"
#include <vector>
#include <list>
#include <string>

struct GeoCoord;

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
	ExpandableHashMap(double maximumLoadFactor = 0.5); // constructor
	~ExpandableHashMap();// destructor; deletes all of the items in the hashmap
	void reset(); // resets the hashmap back to 8 buckets, deletes all items
	int size() const; // return the number of associations in the hashmap
	 // The associate method associates one item (key) with another (value).
	 // If no association currently exists with that key, this method inserts
	 // a new association into the hashmap with that key/value pair. If there is
	 // already an association with that key in the hashmap, then the item
	 // associated with that key is replaced by the second parameter (value).
	 // Thus, the hashmap must contain no duplicate keys.
	void associate(const KeyType& key, const ValueType& value);


	// If no association exists with the given key, return nullptr; otherwise,
	// return a pointer to the value associated with that key. This pointer can be
	// used to examine that value, and if the hashmap is allowed to be modified, to
	// modify that value directly within the map (the second overload enables
	// this). Using a little C++ magic, we have implemented it in terms of the
	// first overload, which you must implement.

	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	ExpandableHashMap(const ExpandableHashMap&) = delete;
	ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

	//template<typename KeyType>
	//unsigned int hasher(const KeyType& g);
	
private:
	struct Node {
		KeyType key;
		ValueType val;
	};

	std::vector<std::list<Node>> m_table;
	int m_numbuks;
	int m_count;
	double MAX_LOAD;
	unsigned int getBucketNumber(const KeyType& key) const
	{
		unsigned int hasher(const KeyType& k); // prototype
		unsigned int h = hasher(key);
		return h % m_numbuks;
	}

	void addOrUpdate(const KeyType& key, const ValueType& value, std::vector<std::list<Node>>& table)
	{
		int bucket = getBucketNumber(key);
		Node addme;
		addme.key = key;
		addme.val = value;

		typename std::list<Node>::iterator it;
		for (it = table[bucket].begin(); it != table[bucket].end(); it++) {
			if (it->key == key) {
				(it->val = value);
				return;
			}
		}
		table[bucket].push_back(addme);
		m_count++;
	}
	
	void rehash() {
		typename std::list<Node>::iterator it;
		m_numbuks = m_numbuks*2;
		std::vector<std::list<Node>> replacement(m_numbuks);
		for (int i = 0; i < m_numbuks/2; i++) {
			typename std::list<Node>::iterator it;
			for (it = m_table[i].begin(); it != m_table[i].end(); it++) {
				addOrUpdate(it->key, it->val, replacement);
			}
		}
		m_table = replacement;
	}
};

template <typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor)
{
	MAX_LOAD = maximumLoadFactor;
	m_numbuks = 8;
	m_count = 0;
	m_table = std::vector<std::list<Node>>(m_numbuks);
}

template <typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
}

template <typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
	for (int i = 0; i < m_numbuks; i++) {
		m_table[i].erase(m_table[i].begin(), m_table[i].end());
	}
	m_numbuks = 8;
	m_table = std::vector<std::list<Node>>(m_numbuks);
}

template <typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
	return m_numbuks;
}

template <typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
	double load = (m_count*1.0 + 1) / (m_numbuks*1.0);
	if (load > MAX_LOAD) {
		rehash();
	}
	addOrUpdate(key, value, m_table);

}

template <typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
	int bucket = getBucketNumber(key);
	
	typename std::list<Node>::const_iterator it;
	for (it = m_table[bucket].begin(); it != m_table[bucket].end(); it++) {
		if (it->key == key) {
			return &(it->val);
		}
	}
    return nullptr;
}

#endif