#ifndef PARTICLE_H
#define PARTICLE_H

class Particle
{
//A particle object to be included in particle systems.
public:

	Particle();

	float m_position;
	float m_velocity;
	float m_mass;
	float m_density;
	float m_viscosity;
	float m_surfaceTension;
	float m_supportRadius;
};

class Water:public Particle
{
	Water();

};

class Ferro: public Particle
{
	Ferro();
};

#endif