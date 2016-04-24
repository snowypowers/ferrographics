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

	glPushMatrix();
	glTranslated(m_center.x(),m_center.y(),m_center.z());
	glutWireSphere(this->m_radius,12,12); //Radius, Slices, Stacks
	//if (m_solid){
	//	glutSolidSphere(this->m_radius,12,12); //Radius, Slices, Stacks
	//} else{
	//	glutWireSphere(this->m_radius,12,12); //Radius, Slices, Stacks
	//}
	glPopMatrix();

}

bool ForceSphere::intersect(Vector3f point){
	if ((point-this->m_center).absSquared()<=pow(this->m_radius,2)){
		return true;
	}
	return false;
}

Vector3f ForceSphere::polarize(Vector3f position){
	Vector3f dir = (position-this->m_center).normalized();
	float r = (position-this->m_center).absSquared();

	if((dir.x()==0)||(dir.z()==0)){
		return r*this->m_amplitude*dir; //Full magnetic force to form spike
	}

	float theta = std::acos(Vector3f::dot(dir,Vector3f::UP)); //Angle in radians from normal
	float phi = std::atan2(dir.x(),dir.z());	//Angle in radians from Z-axis

	float t_weight = cos(theta*this->m_peaks);
	float p_weight = cos(phi*this->m_peaks*4);

	//printf("T weight = %f",t_weight);
	//printf("P weight = %f",p_weight);

	if (t_weight <0.1){
		t_weight = 0.1;
	}
	if (p_weight <0.1){
		p_weight = 0.1;
	}
	return (r * this->m_amplitude * t_weight * p_weight)*dir;
}

void ForceSphere::move(Vector3f translate){
	this->m_center = this->m_center+translate;
}

bool ForceSphere::getSolid(){
	return this->m_solid;
}

void ForceSphere::setSolid(bool s){
	m_solid=s;
	printf("%s", this->m_solid ? "solid" : "wire");
}
