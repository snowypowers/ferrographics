
#include "pendulumSystem.h"
#include "Spring.h"

PendulumSystem::PendulumSystem() {
	m_vVecState = vector<Vector3f>();
	m_springs = vector<Spring>();
}

PendulumSystem::PendulumSystem(int numParticles):ParticleSystem(numParticles)
{
	m_numParticles = numParticles;
	m_vVecState = vector<Vector3f>();
	m_springs = vector<Spring>();

	//Constants
	k = 25.0f;
	setMass(0.5f);
	setDrag(1.0f);

	// fill in code for initializing the state based on the number of particles
	//We construct a string of particles dangling downwards, spaced 1 unit apart.
	for (int i = 0; i < m_numParticles; i++) {
		//Insert particle -i space away (in y-direction) from origin 
		m_vVecState.push_back(Vector3f(0.3f * i,-0.1f * i,0.0f));
		//Insert velocity of zero
		m_vVecState.push_back(Vector3f(0.0f,-0.0f,0.0f));
		// for this system, we care about the position and the velocity
		if (i>0) {
			Spring s = Spring(0.5, k, i-1, i, 0);
			this->m_springs.push_back(s);
		}
	}


	m_anchors.push_back(0);
}


// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> PendulumSystem::evalF(vector<Vector3f> state)
{
	/*printf("STATE: \n");
	for (unsigned z=0;z <state.size();z++) {
		state[z].print();
	}*/
	vector<Vector3f> f;
	Vector3f p, v;
	// YOUR CODE HERE
	//for each particle
	for (unsigned i=0;i<state.size();i+=2) {
		//extract the particle's position and velocity
		p = state[i];
		v = state[i+1];
		//Gravity
		Vector3f f_gravity = mass * Vector3f(0.0f, -9.81f, 0.0f);
		//Drag
		Vector3f f_drag = (-1.0 * dragCoefficient) * v;
		f.push_back(v);
		f.push_back(f_gravity + f_drag);
	}
	//f now contains 2 vectors per particle - velocity and force

	Vector3f p1, p2, d;
	//Calculate spring force for each spring
	for (unsigned j=0;j<m_springs.size();j++) {
		Spring s = m_springs[j];
		p1 = state[s.getParticle1() * 2];
		p2 = state[s.getParticle2() * 2];
		d  = p1 - p2;
		Vector3f f1 = -s.getK() * (d.abs() - s.getRestLength()) * d.normalized();
		//f1.print();
		f[s.getParticle1() * 2 + 1] += f1;
		d = p2 - p1;
		Vector3f f2 = -s.getK() * (d.abs() - s.getRestLength()) * d.normalized();
		//f2.print();
		f[s.getParticle2() * 2 + 1] += f2;
	}
	//Divide net force by mass to find acceleration
	for (unsigned k=1;k<f.size();k+=2) {
		f[k] = f[k] / mass;
	}
	//Set particles to be anchor
	for (unsigned a=0;a<m_anchors.size();a++) {
		f[m_anchors[a]*2] = Vector3f();
		f[m_anchors[a]*2 + 1] = Vector3f();
	}

	return f;
}


// render the system (ie draw the particles)
void PendulumSystem::draw()
{
	for (int i = 0; i < m_numParticles; i++) {
		Vector3f pos = m_vVecState[i*2];//  position of particle i. YOUR CODE HERE
		//printf("%d: ", i);
		//pos.print();
		//this->m_vVecState[i*2+1].print();
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
	}
	/*This draws the springs as cubes
	for (int j=0;j < m_springs.size();j++) {
		Spring s = m_springs[j];
		Vector3f p1 = m_vVecState[s.getParticle1()*2];
		Vector3f p2 = m_vVecState[s.getParticle2()*2];
		Vector3f mid = (p2-p1) / 2.0;
		glPushMatrix();
		glTranslatef(p1[0] + mid[0], p1[1] + mid[1], p1[2] + mid[2]);
		glutSolidCube(0.05f);
		glPopMatrix();
	}*/
}
