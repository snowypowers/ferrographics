#include "Box.h"
#include <cmath>

Box::Box() {
	//Default side length of 1
	m_side = 1.0f;
	//Default center of (0,0,0)
	m_c = Vector3f(1,1,1);
	//Default direction up in +y
	m_up = Vector3f(0, 1, 0);
	//Closed box
	m_covered = true;
	m_points = vector<Vector3f>();
	cal();
}

Box::Box(float side, Vector3f c, Vector3f up, bool covered) {
	m_side = side;
	m_c = c;
	m_up = up;
	m_covered = covered;
	m_points = vector<Vector3f>();
	cal();
}

//Check for collision with given particle
bool Box::collide(Vector3f &position) {
	for (unsigned i=0; i<3;i++) {
		if (abs(position[i] - m_c[i]) > m_side/2) {
			return true;
			//direction[i] = position[i] - m_c[i];
		}
	}
	return false;
}

void Box::initTable(float cellSize) {
	m_cellSize = cellSize;
	m_tableLength = floor(m_side / cellSize);
	int tableSize = pow(floor(m_side / cellSize), 3);
	table = new HashNode* [tableSize] ();
	printf("Table Size: %d", tableSize);
};

void Box::insert(vector<Vector3f> &points) {
	for (int i=0;i< points.size();i+=2) {
			int key = hash(points[i]);
			HashNode* n = new HashNode(key, i/2);
			if (table[key] != NULL) {
				n->setNext(table[key]);
			}
			table[key] = n;
		}
};

int Box::hash(Vector3f p) {
	return floor(p[0]/m_cellSize) + floor(p[1]/m_cellSize) * m_tableLength + floor(p[2]/m_cellSize) * m_tableLength * m_tableLength;
};

int Box::hash(int i1, int i2, int i3) {
	int limit = m_tableLength - 1;
	int i = min((int)floor(i1/m_cellSize), limit);
	int j = min((int)floor(i2/m_cellSize), limit);
	int k = min((int)floor(i3/m_cellSize), limit);
	return  i + j * m_tableLength + k * m_tableLength * m_tableLength;
};


vector<int> Box::findNeighbours(Vector3f pos) {
	int r [3] = {floor(pos[0]/m_cellSize), floor(pos[1]/m_cellSize), floor(pos[2]/m_cellSize)};
	vector<int> results = vector<int>();
	int limit = m_tableLength - 1;
	for (int i=-1;i<=1;++i) {
		for (int j=-1;j<=1;++j) {
			for (int k=-1;k<=1;++k) {
				int i1 = max(0, min(i+r[0], limit));
				int i2 = max(0, min(j+r[1], limit));
				int i3 = max(0, min(k+r[2], limit));
				//printf("Hashing: %d %d %d\n", i+r[0], j+r[1], k+r[2]);
				int key = i1 + i2 * m_tableLength + i3 * m_tableLength * m_tableLength;
				//printf("find hash value %d\n", key);
				if (key < 0) {continue;}
				
				HashNode* n = table[key];
				while (n != NULL) {
					int v = n->getValue();
					results.push_back(v);
					n= n->getNext();
				}
			}
		}
	}
	return results;
};

void Box::clear() {
	//Only clear buckets but not table
	for (int i = 0; i < m_side; ++i) {
        HashNode *entry = table[i];
        while (entry != NULL) {
            HashNode *prev = entry;
            entry = entry->getNext();
            delete prev;
        }
        table[i] = NULL;
    }
};

void Box::getPoints(float x[]){
	//float points[6] = {m_points[0].x(),m_points[1].x(),m_points[3].y(),m_points[4].y(),m_points[3].z(),m_points[5].z()};
	x[0] = m_points[4].x();	//x
	x[1] = m_points[2].x();	//-x
	x[2] = m_points[4].y();	//y
	x[3] = m_points[2].y();	//-y
	x[4] = m_points[4].z();	//z
	x[5] = m_points[2].z();	//-z
}

void Box::cal() {
	m_points.clear();
	Vector3f one = m_c + Vector3f(-m_side/2, m_side/2, -m_side/2);	//x- y+ z-
	Vector3f two = m_c + Vector3f(m_side/2, m_side/2, -m_side/2);	//x+ y+ z-
	Vector3f three = m_c + Vector3f(m_side/2, m_side/2, m_side/2);	//x+ y+ z+
	Vector3f four = m_c + Vector3f(-m_side/2, m_side/2, m_side/2);	//x- y+ z+
	Vector3f five = m_c + Vector3f(-m_side/2, -m_side/2, -m_side/2);//x- y- z-
	Vector3f six = m_c + Vector3f(m_side/2, -m_side/2, -m_side/2);	//x+ y- z-
	Vector3f seven = m_c + Vector3f(m_side/2, -m_side/2, m_side/2);
	Vector3f eight = m_c + Vector3f(-m_side/2, -m_side/2, m_side/2);

	m_points.push_back(one);
	m_points.push_back(two);
	m_points.push_back(three);
	m_points.push_back(four);
	m_points.push_back(five);
	m_points.push_back(six);
	m_points.push_back(seven);
	m_points.push_back(eight);


};

void Box::draw() {
	if (m_covered) {
		drawQuad(m_points[0], m_points[1], m_points[2], m_points[3]);
	}
	drawQuad(m_points[1], m_points[0], m_points[4], m_points[5]);
	drawQuad(m_points[2], m_points[1], m_points[5], m_points[6]);
	drawQuad(m_points[3], m_points[2], m_points[6], m_points[7]);
	drawQuad(m_points[0], m_points[3], m_points[7], m_points[4]);
	drawQuad(m_points[7], m_points[6], m_points[5], m_points[4]);

}

void Box::drawQuad(Vector3f a, Vector3f b, Vector3f c, Vector3f d) {
	glColor3f(50,50,50);
	glBegin(GL_QUADS);
	glVertex3f(a[0],a[1],a[2]);
	glVertex3f(b[0],b[1],b[2]);
	glVertex3f(c[0],c[1],c[2]);
	glVertex3f(d[0],d[1],d[2]);
	glEnd();
}