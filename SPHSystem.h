#ifndef SPHSYSTEM_H
#define SPHSYSTEM_H

#include <vecmath.h>
#include <vector>
#ifdef _WIN32
#include "GL/freeglut.h"
#else
#include <GL/glut.h>
#endif

#include "particleSystem.h"
#include "SpatialHash.h"
#include "Material.h"
#include "Weights.h"
#include "Box.h"

class SPHSystem: public ParticleSystem
{
public:
	SPHSystem();
	vector<Vector3f> evalF(vector<Vector3f> state);
	void checkCollision();
	void draw();


protected:
	Box box;
	Material* m;
	float m_mass;
	SpatialHash* hash;


};
#endif