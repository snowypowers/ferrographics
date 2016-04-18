#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include <vecmath.h>
#include "Box.h"

using namespace std;

class ParticleSystem
{
public:

	ParticleSystem(int numParticles=0);

	int m_numParticles;
	
	// for a given state, evaluate derivative f(X,t)
	virtual vector<Vector3f> evalF(vector<Vector3f> state) = 0;
	
	// getter method for the system's state
	vector<Vector3f> getState(){ return m_vVecState; };
	
	// setter method for the system's state
	void setState(const vector<Vector3f>  & newState) { m_vVecState = newState; };
	
	virtual void draw() = 0;

	virtual void checkCollision();

	void addForceField(Vector3f f);

	void removeForceField(Vector3f f);

	void print();
	
	bool mesh;
	
protected:

	vector<Vector3f> m_vVecState;
	Vector3f m_forceField;
	Box box;

};

#endif
