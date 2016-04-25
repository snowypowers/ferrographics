#ifndef BOX_H
#define BOX_H

#include <algorithm>
#include <vecmath.h>
#include<vector>
#include "HashNode.h"

#ifdef _WIN32
#include "GL/freeglut.h"
#else
#include <GL/glut.h>
#endif

using namespace std;
/*
Box implementation
This is the hollow square box.

@params
float side: length of side
Vector3f c: Center of the box
Vector3f up: Upward direction of the box
bool top: True for closed box, False for box with open top
*/
class Box
{
public:
	//Default Constructor
	Box();

	//Constructor
	Box(float side, Vector3f c, Vector3f up, bool covered);

	float getSide() {return m_side;}

	void draw();

	void cal();

	bool collide(Vector3f &position);

	void getPoints(float x[]);

	void initTable(float cellSize);

	void insert(vector<Vector3f> &points);

	int hash(Vector3f p);

	void clear();

	vector<int> findNeighbours(Vector3f pos);


protected:
	vector<Vector3f> m_points;
	float m_side;
	Vector3f m_c;
	Vector3f m_up;
	bool m_covered;
	float m_cellSize;
	int m_tableLength;
	HashNode ** table;

	void drawQuad(Vector3f a, Vector3f b, Vector3f c, Vector3f d);

};
#endif