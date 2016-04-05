#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include <vecmath.h>
#include <vector>
#ifdef _WIN32
#include "GL/freeglut.h"
#else
#include <GL/glut.h>
#endif

#include "pendulumSystem.h"

class ClothSystem: public PendulumSystem
{
///ADD MORE FUNCTION AND FIELDS HERE
public:
	ClothSystem(int length, int width, float distance);

	vector<Vector3f> evalF(vector<Vector3f> state);
	
	void draw();

	Vector3f getParticle(int l, int w);
	Vector3f getParticle(int index);

private:
	int m_length, m_width, m_wind;
};


#endif
