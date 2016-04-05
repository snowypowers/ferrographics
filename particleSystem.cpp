#include "particleSystem.h"
ParticleSystem::ParticleSystem(int nParticles):m_numParticles(nParticles){
	m_forceField = Vector3f();
	mesh = false;
}

void ParticleSystem::print() {
	for (unsigned z=0;z<m_vVecState.size();z++) {
		printf("%d:",z);
		m_vVecState[z].print();
	}
}

void ParticleSystem::addForceField(Vector3f f) {
		m_forceField += f;
}

void ParticleSystem::removeForceField(Vector3f f) {
	m_forceField -= f;
}