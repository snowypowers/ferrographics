
#include "simpleSystem.h"

using namespace std;

SimpleSystem::SimpleSystem()
{
	m_vVecState = vector<Vector3f>();
	m_vVecState.push_back(Vector3f(1,1,0));
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> SimpleSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;
	// YOUR CODE HERE
	Vector3f v;
	for (unsigned i=0;i< state.size();i++) {
		//v = {-y, x, 0}.T;
		v = Vector3f();
		v[0] = -state[i][1];
		v[1] = state[i][0];
		v[2] = 0.0f;
		f.push_back(v);
	}
	return f;
}

// render the system (ie draw the particles)
void SimpleSystem::draw()
{
	for (unsigned i=0;i<m_vVecState.size();i++) {
		Vector3f pos = m_vVecState[i];//YOUR PARTICLE POSITION
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
	}
}
