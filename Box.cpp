#include "Box.h"
#include <cmath>

Box::Box() {
	//Default side length of 1
	m_side = 1.0f;
	//Default center of (0,0,0)
	m_c = Vector3f();
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
bool Box::collide(Vector3f position) {
	bool result = false;
	Vector3f direction = Vector3f();
	for (unsigned i=0; i<3;i++) {
		if ((position - m_c).abs() > m_side/2) {
			result = true;
			//direction[i] = position[i] - m_c[i];
		}
	}
	if (!result) {
		return false;
	} else {

	}
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


}

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