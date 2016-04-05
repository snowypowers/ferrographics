#ifndef PENDULUMSYSTEM_H
#define PENDULUMSYSTEM_H

#include <vecmath.h>
#include <vector>
#ifdef _WIN32
#include "GL/freeglut.h"
#else
#include <GL/glut.h>
#endif

#include "particleSystem.h"
#include "Spring.h"

class PendulumSystem: public ParticleSystem
{
public:
	PendulumSystem();
	PendulumSystem(int numParticles);
	
	vector<Vector3f> evalF(vector<Vector3f> state);
	
	void draw();

	void setMass(float m) {mass = m;};

	void setDrag (float d) {dragCoefficient = d;};

	void addAnchor(int i) {m_anchors.push_back(i);}

	void clearAnchors() {m_anchors.clear();}
	
protected:
	//Mass
	float mass;
	//Drag Coefficient
	float dragCoefficient;
	//Spring Constant
	float k;
	//List of springs
	vector<Spring> m_springs;

	vector<int> m_anchors;
};

#endif
