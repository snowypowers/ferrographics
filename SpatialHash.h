#ifndef SPATIALHASH_H
#define SPATIALHASH_H

#include <cstddef>
#include "vecmath.h"
#include "HashNode.h"

#define P1 73856093
#define P2 19349663
#define P3 83492791

class SpatialHash {


public:
	SpatialHash(int num_particles, float cellSize) {
		m_cellSize = cellSize;
		m_size = next_prime(num_particles * 2);
		printf("Size is %d\n", m_size);
		table = new HashNode<int, int>* [m_size];
		for (int i=0;i<m_size;i++) {
			table[i] = NULL;
		}
	};

	 ~SpatialHash() {
        // destroy all buckets one by one
        for (int i = 0; i < m_size; ++i) {
            HashNode<int, int> *entry = table[i];
            while (entry != NULL) {
                HashNode<int, int> *prev = entry;
                entry = entry->getNext();
                delete prev;
            }
            table[i] = NULL;
        }
        // destroy the hash table
        delete [] table;
    }

	 int* rhat(Vector3f pos) {
		int r0 = floor(pos[0] / m_cellSize);
		int r1 = floor(pos[1] / m_cellSize);
		int r2 = floor(pos[2] / m_cellSize);
		int result [] = {r0,r1,r2};
		return result;
	 };

	//Takes in the particle index and position
	void insert(int index, Vector3f pos) {
		int* r = rhat(pos);
		int key = ( (r[0] * P1)^(r[1] * P2) ^ (r[2] * P3)) % m_size;
		HashNode<int, int>* node = new HashNode<int, int>(key, index);
		HashNode<int, int>* starter = table[key];
		if (starter != NULL) {
			starter->getLast()->setNext(node);
		} else {
			table[key] = node;
		}
	};

	HashNode<int, int>* get(int x, int y, int z) {
		int key = ( (x * P1)^(y * P2) ^ (z * P3)) % m_size;
		return table[key];
	};

	void remove(int key, int index) {
		HashNode<int, int>* node = table[key];
		if(node->getValue() == index) {
			table[key] = node->getNext();
			delete node;
			node = table[key];
		}
		while (node->getNext() != NULL) {
			HashNode<int, int>* next = node->getNext();
			if (next->getKey() == index) {
				node->setNext(next->getNext());
				delete next;
			} else {
				node = node->getNext();
			}
				

		}

	};

	void clear() {
		//Only clear buckets but not table
		for (int i = 0; i < m_size; ++i) {
            HashNode<int, int> *entry = table[i];
            while (entry != NULL) {
                HashNode<int, int> *prev = entry;
                entry = entry->getNext();
                delete prev;
            }
            table[i] = NULL;
        }
	};


	vector<int> findNeighbours(Vector3f pos, float h) {
		int* min =  rhat(pos - h);
		int* max =  rhat(pos + h);
		vector<int> results = vector<int>();
		for (int i=min[0];i<=max[0];i++) {
			for (int j=min[1];j<=max[1];j++) {
				for (int k=min[2];k<=max[2];k++) {
					HashNode<int, int>* node = get(i,j,k);
					while (node != NULL) {
						results.push_back(node->getValue());
						node = node->getNext();
					}
				}
			}
		}

		return results;
	};

	bool is_prime(std::size_t x) {
    for (std::size_t i = 3; true; i += 2)
    {
        std::size_t q = x / i;
        if (q < i)
            return true;
        if (x == q * i)
            return false;
    }
    return true;
	};

	std::size_t next_prime(std::size_t x) {
		if (x <= 2)
			return 2;
		if (!(x & 1))
			++x;
		for (; !is_prime(x); x += 2)
			;
		return x;
	};
	
protected:
	int m_size;
	float m_cellSize;
	HashNode<int, int> ** table;

};


#endif