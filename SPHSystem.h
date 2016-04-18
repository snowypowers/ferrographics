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

class SPHSystem: public ParticleSystem
{
public:
	SPHSystem();
	vector<Vector3f> evalF(vector<Vector3f> state);
	void draw();


protected:
	Material* m;
	float m_mass;
	SpatialHash* hash;


};
#endif