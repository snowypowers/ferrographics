#ifndef MATERIAL_H
#define MATERIAL_H

#define PI 3.14159265359

class Material {
public:
	Material(float density, float vol, int particles, float h) {
			m_density = density;
			m_volume = vol;
			m_particles = particles;
			//m_mass = 0.02f;
			m_mass = density * (vol / particles);
			m_radius = 0.01f;
			//m_radius = pow((float) ((3.0f * m_mass) / (4.0f * PI * m_density)), (float) (1.0/3.0));
			m_h = h;
			m_support = pow((float) ((3.0f * vol * 20.0f) / (4.0f * PI * particles))  , 1/3.0f);
	};

	float getMass() {
		return m_mass;
	};

	float getDensity() {
		return m_density;
	};

	float getNumParticles() {
		return m_particles;
	};

	float getVolume() {
		return m_volume;
	};

	float getH() {
		return m_h;
	};

	float getRadius() {
		return m_radius;
	};
	float getRestPressure() {
		return m_restPressure;
	};

	float getK() {
		return m_k;
	};

	float getViscosity() {
		return m_viscosity;
	};

	float getSupport() {
		return m_support;
	};

	float getSurfaceTension() {
		return m_surfaceTension;
	};

protected:
	float m_mass;
	float m_density;
	float m_volume;
	int m_particles;
	float m_radius;
	float m_h;
	float m_restPressure;
	float m_k;
	float m_viscosity;
	float m_support;
	float m_surfaceTension;

};

class Water:public Material {
public:
	Water(float density, float vol, int particles, float h):Material(density, vol, particles, h) {
		m_restPressure = 1000.0f;
		m_k = 3.5f;
		m_viscosity = 3.5f;
		m_support = 0.1f;
		m_surfaceTension = 0.07f;
	}
};

#endif