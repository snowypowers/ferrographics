#include "Spring.h"

Spring::Spring(float rest_length, float spring_constant, int p1, int p2) {
	this->r = rest_length;
	this->k = spring_constant;
	this->p1 = p1;
	this->p2 = p2;
}

Spring::Spring(float rest_length, float spring_constant, int p1, int p2, int t) {
	this->r = rest_length;
	this->k = spring_constant;
	this->p1 = p1;
	this->p2 = p2;
	setType(t);
}
	int Spring::getParticle1() {
		return this->p1;	
	};

	int Spring::getParticle2(){
		return this->p2;
	};

	float Spring::getRestLength(){
		return this->r;
	};

	float Spring::getK(){
		return this->k;
	};

	void Spring::setType(int i) {
		type = i;
	}

	int Spring::getType() {
		return type;
	}
