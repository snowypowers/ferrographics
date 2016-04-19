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
	SPHSystem(Material* mat, int numParticles, bool empty);
	void addParticle(Vector3f pos, Vector3f velo);
	vector<Vector3f> evalF(vector<Vector3f> state);
	void checkCollision();
	void draw();
	Box getBox() {return box;}


protected:
	Box box;
	Material* m;
	SpatialHash* hash;


};
#endif