#ifndef SPRING_H
#define SPRING_H


#include <vecmath.h>
#include <vector>

class Spring
{

public:
	Spring(float rest_length, float spring_constant, int p1, int p2);

	Spring(float rest_length, float spring_constant, int p1, int p2, int type);

	void setType(int i);

	int getType();

	int getParticle1();

	int getParticle2();

	float getRestLength();

	float getK();

protected:
	//rest length
	float r;
	//spring constant
	float k;
	//Index of particles, type of spring
	int p1, p2, type;
};

#endif