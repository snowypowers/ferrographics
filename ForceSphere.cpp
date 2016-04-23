#include "ForceSphere.h"
#include <cmath>

ForceSphere::ForceSphere(void)
{
	//Default of Center at Origin
	this->m_center = Vector3f();
	//default radius of 0.5 
	this->m_radius = 0.5;
	//default amplitude of 0.1
	this->m_amplitude = 0.1;
	//default number of peaks per 
	this->m_peaks = 3;
}

ForceSphere::ForceSphere(Vector3f center, float radius, float amplitude, int peaks)
{
	this->m_center = center;
	this->m_radius = radius;
	this->m_amplitude = amplitude;
	this->m_peaks = peaks;
}


ForceSphere::~ForceSphere(void)
{
}

void ForceSphere::draw(){
	glColor3d(1,0,0);
	Matrix4f t = Matrix4f::translation(this->m_center);
	glLoadMatrixf(t);
	if (this->solid){
		glutSolidSphere(this->m_radius,12,12); //Radius, Slices, Stacks
	} else{
		glutWireSphere(this->m_radius,12,12); //Radius, Slices, Stacks
	}
}

bool ForceSphere::intersect(Vector3f point){
	if ((point-this->m_center).absSquared()<=pow(this->m_radius,2)){
		return true;
	}
	return false;
}
