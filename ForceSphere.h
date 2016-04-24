#ifndef FSPHERE_H
#define FSPHERE_H

#include <vecmath.h>
#include<vector>

#ifdef _WIN32
#include "GL/freeglut.h"
#else
#include <GL/glut.h>
#endif

using namespace std;

/*
Magnectic Field Sphere
This is the approximated model of magnetic field lines that Ferrofluids follow to form its shape.

@params
Vector3f Center: Center of Sphere
float Radius: Radius of Sphere
float Amplitude: Amplitude of Cos Wave kernel
int Peaks: Number of peaks per quarter of sphere 
*/

class ForceSphere
{
public:
	ForceSphere(void);
	ForceSphere(Vector3f center, float radius, float amplitude, int peaks);
	~ForceSphere(void);
	void draw();
	bool intersect(Vector3f point); //Point in world coordinates

	Vector3f polarize(Vector3f position);

	void move(Vector3f translate);
	bool getSolid();
	void setSolid(bool solid);
	
	
protected:
	Vector3f m_center;
	float m_radius;
	float m_amplitude;
	int m_peaks;
	bool m_solid;
	
};
#endif

