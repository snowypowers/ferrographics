#ifndef SPATIALHASH_H
#define SPATIALHASH_H

#include <cstddef>
#include <algorithm>
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
		table = new HashNode* [m_size] ();
	};

	 ~SpatialHash() {
        // destroy all buckets one by one
        for (int i = 0; i < m_size; ++i) {
            HashNode *entry = table[i];
            while (entry != NULL) {
                HashNode *prev = entry;
                entry = entry->getNext();
                delete prev;
            }
            table[i] = NULL;
        }
        // destroy the hash table
        delete [] table;
    };

	 vector<int> rhat(Vector3f pos) {
		int r0 = floor(pos[0] / m_cellSize);
		int r1 = floor(pos[1] / m_cellSize);
		int r2 = floor(pos[2] / m_cellSize);
		vector<int> result = vector<int>();
		result.push_back(r0);
		result.push_back(r1);
		result.push_back(r2);
		return result;
	 };

	 int hash(Vector3f &pos) {
		vector<int> r = rhat(pos);
		int result = ( ( (r[0] * P1) ^ (r[1] * P2) ^ (r[2] * P3)) )% m_size;
		if (result < 0) {printf("ERROR: %d NEGATIVE KEY!!!\n", result);}
		if (result >= m_size) {printf("ERROR: OUT OF BOUNDS!!!\n");}
		return result;
	 };

	//Takes in the particle index and position
	void insert(int index, Vector3f pos) {
		unsigned int key = hash(pos);
		HashNode* node = new HashNode(key, index);
		if (table[key] != NULL) {
			node->setNext(table[key]);
		}
		table[key] = node;
	};

	void insert(vector<Vector3f> &points) {
		for (int i=0;i< points.size();i+=2) {
			int key = hash(points[i]);
			HashNode* n = new HashNode(key, i/2);
			if (table[key] != NULL) {
				n->setNext(table[key]);
			}
			table[key] = n;
		}
	};

	void overview() {
		int count = 0;
		for (int i=0;i<m_size;++i) {
			printf("KEY: %d | ", i);
			HashNode* n = table[i];
			while (n != NULL) {
				printf("%d ", n->getValue());
				count ++;
				n = n->getNext();
			}
			printf("\n");
		}
		printf("COUNT: %d\n", count);
	};

	void remove(int key, int index) {
		HashNode* node = table[key];
		if(node->getValue() == index) {
			table[key] = node->getNext();
			delete node;
			node = table[key];
		}
		while (node->getNext() != NULL) {
			HashNode* next = node->getNext();
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
            HashNode *entry = table[i];
            while (entry != NULL) {
                HashNode *prev = entry;
                entry = entry->getNext();
                delete prev;
            }
            table[i] = NULL;
        }
	};


	vector<int> findNeighbours(Vector3f pos, float h) {
		vector<int> r = rhat(pos);
		printf("RHAT: %d %d %d\n", r[0], r[1], r[2]);
		vector<int> results = vector<int>();
		for (int i=r[0]-1;i<=r[0]+1;++i) {
			for (int j=r[1]-1;j<=r[1]+1;++j) {
				for (int k=r[2]-1;k<=r[2]+1;++k) {
					//printf("Hashing: %d %d %d\n", i, j, k);
					int key = hash(Vector3f(i,j,k));
					if (key < 0) {continue;}
					if (key >= m_size) {printf("ERROR: OUT OF BOUNDS %d", key);}
					//printf("find hash value %d ", key);
					HashNode* n = table[key];
					while (n != NULL) {
						int v = n->getValue();
						//printf(" %d ", v);
						if (std::find(results.begin(), results.end(), v) == results.end()) {
							//printf("Pushing, ");
								results.push_back(v);
						}
						n= n->getNext();
					}
					//printf("results is %d long\n", results.size());
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
	HashNode ** table;

};


#endif