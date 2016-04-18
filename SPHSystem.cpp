#include "SPHSystem.h"

SPHSystem::SPHSystem() :ParticleSystem(){
	m = new Water(1000, 0.01, 500, 0.0457); 
	m_numParticles = 100;
	for (int i=0;i<(m_numParticles*2);i++) {
		if (i%2 == 0) {
			m_vVecState.push_back(Vector3f(i/100,i/100,i/100));
		} else {
			m_vVecState.push_back(Vector3f());
		}
		
	}
	hash = new SpatialHash(100, 0.01);
};

SPHSystem::SPHSystem(Material* mat, int numParticles, bool empty):ParticleSystem() {
	m = mat;
	m_numParticles = numParticles;
	if (!empty) {
		for (int i=0;i<(m_numParticles*2);i++) {
			if (i%2 == 0) {
				m_vVecState.push_back(Vector3f(i/100,i/100,i/100));
			} else {
				m_vVecState.push_back(Vector3f());
			}
		}
	}
	hash = new SpatialHash(numParticles, 0.01);
}

void SPHSystem::addParticle(Vector3f pos, Vector3f velo) {
	m_vVecState.push_back(pos);
	m_vVecState.push_back(velo);
};

vector<Vector3f> SPHSystem::evalF(vector<Vector3f> state) {
	//printf("Start eval\n");
	int p, i;//Counter
	hash->clear();
	vector<Vector3f> output = vector<Vector3f>();
	//printf("Cleared Hash\n");
	//Insert all particles into hash
	for (p = 0;p<state.size();p+=2) {//p represents location of particle in state. p/2 is the index
		hash->insert(p/2,state[p]);
	}
	//printf("Inserted Hash\n");
	//p is now the size of the hash
	vector<vector<int>> neighboursArray = vector<vector<int>> ();
	vector<float> mass_densityArray = vector<float>();
	vector<float> pressureArray = vector<float>();
	//Calculate density and pressure for each particle
	for (i=0;i<p;i++) {
		float mass_density = 0;
		float pressure = 0;
		//find all possible neighbours
		vector<int> neighbours = hash->findNeighbours(i, 0.0457);
		//Check if they are nearby
		for (int j=0;j<neighbours.size();j++) {
			Vector3f pos = state[neighbours[i]*2];
			//If neighbour particle is within support radius of particle i
			if (abs(pos.abs() - state[i*2].abs()) <= m->getH()) {
				Vector3f diff = state[i*2] - pos;
				mass_density += m_mass * Weights::default(diff,m->getH());
			} else {
			//Set invalid neighbour to -1
				neighbours[i] = -1;
			}
		}
		//Calculate pressure using rest pressure
		pressure = m ->getK() * ( mass_density - m->getRestPressure());
		neighboursArray.push_back(neighbours);
		mass_densityArray.push_back(mass_density);
		pressureArray.push_back(pressure);
	}
	
	for (i=0;i<p;i++) {
		//compute internal forces for each particle
		vector<int> neighbours = neighboursArray[i];
		Vector3f pressureForce = Vector3f();
		Vector3f visForce = Vector3f();
		Vector3f pi = pressureArray[i];
		Vector3f mi = mass_densityArray[i];
		Vector3f vi = state[i*2 + 1];
		for (int j=0;j<neighbours.size();j++) {
			int index = neighbours[j];
			if (index == -1) {continue;}
			if (index == i) {continue;}
			Vector3f diff = state[i*2] - state[index*2];
			Vector3f pj = pressureArray[index];
			float mj = mass_densityArray[index];
			pressureForce -= ( (pi + pj) / 2 ) * (mj / pj) * Weights::pressure(diff, m->getH());
			visForce += (state[index*2 + 1] - vi) * (mj / pj) * Weights::vis(state[index*2], m->getH());
		}
		visForce *= m->getViscosity();
		//External forces
		Vector3f gravityForce = mi * 9.81;

		//Sum up all forces and put into output
		//Push in velocity
		output.push_back(state[i*2 + 1]);
		Vector3f finalForce = pressureForce = visForce + gravityForce;
		output.push_back(finalForce / mi);
	}

	return output;

	
};

void SPHSystem::draw() {
	float r = m->getRadius();
	for (int i=0;i<m_vVecState.size();i+=2) {
		Vector3f pos = m_vVecState[i*2];
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(r,5.0f,5.0f);
		glPopMatrix();
	}
};

