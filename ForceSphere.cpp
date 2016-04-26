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
	Vector3f diff = position-this->m_center;
	if(diff.abs()==0){
		//printf("Im zero!\n");
		return Vector3f();
	}
	Vector3f dir = diff.normalized();
	float magnitdue = diff.absSquared();
	if((dir.x()==0)||(dir.z()==0)||(dir.y()==0)){
		//return r*this->m_amplitude*dir; //Full magnetic force to form spike
		return Vector3f();
	}

	float theta = std::acos(Vector3f::dot(dir,Vector3f::UP)); //Angle in radians from normal
	float phi = std::atan2(dir.x(),dir.z());	//Angle in radians from Z-axis
	//printf("Position of particle:");
	//position.print();
	//printf("Theta = %f	Phi = %f\n",theta,phi);
	float t_weight = (2.0-(1.0+cos(theta*4*this->m_peaks)))/2.0;
	float p_weight = (2.0-(1.0+cos(phi*this->m_peaks*4)))/2.0;
	//printf("T Weight = %f	P Weight = %f\n",t_weight,p_weight);
	//Vector3f force =(this->m_amplitude *((exp(magnitdue)-1.0)/magnitdue )*(1-t_weight)*(1-p_weight))*-1*dir;
	//force.print();
	return (this->m_amplitude*((exp(magnitdue)-1.0)/this->m_radius )*p_weight*t_weight)*-1*dir;
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

void ForceSphere::setAmplitude(float amplitude){
	this->m_amplitude+=(amplitude);
}

float ForceSphere::getAmplitude(){
	return this->m_amplitude;
}