#include "ClothSystem.h"
#include <math.h>

//TODO: Initialize here
//Constructs a cloth consisting of <length> particles long and <width> particles tall in the xz-plane. Particles are spaced <distance> apart.
ClothSystem::ClothSystem(int l, int w, float distance):PendulumSystem()
{
	//Setup params
	m_length = l;
	m_width = w;
	m_numParticles = l * w;

	//Constants
	float k = 15.0f;
	setMass(0.25f);
	setDrag(2.0f);

	float shear = sqrt(2.0);
	for (unsigned i=0;i<l;i++) {//x
		for (unsigned j=0;j<w;j++) {//z
			m_vVecState.push_back(Vector3f(distance * i, 0.0f, distance * j));
			//Zero initial velocity
			m_vVecState.push_back(Vector3f(0.0f,0.0f,0.0f));

			
			if (j>0) {
				//Horizontal Structural Springs
				m_springs.push_back(Spring(distance, k, i*w+j-1, i*w+j, 0));
			}
			
			if (i>0) {
				//Vertical Structural Springs
				m_springs.push_back(Spring(distance, k, (i-1)*w+j, i*w+j, 0));
			}

			if (i>0 && j>0) {
				//Backslash Shear Springs
				m_springs.push_back(Spring(distance*shear, k, (i-1)*w+j-1, i*w+j, 1));
			}

			if (i>0 && j<w-1) {
				//Forwardslash Shear Springs
				m_springs.push_back(Spring(distance*shear, k, (i-1)*w+j+1, i*w+j, 1));
			}

			if (j>1) {
				//Horizontal Flex Springs
				m_springs.push_back(Spring(distance*2, k, i*w+j-2, i*w+j, 2));
			}

			if (i>1) {
				//Vertical Flex Springs
				m_springs.push_back(Spring(distance*2, k, (i-2)*w+j, i*w+j, 2));
			}
		}
	}


	addAnchor(0);
	addAnchor(l-1);

}

Vector3f ClothSystem::getParticle(int l, int w) {
	return m_vVecState[(l*m_width*2) + w * 2];
}

Vector3f ClothSystem::getParticle(int index) {
	return m_vVecState[index*2];
}
// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> ClothSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> output = PendulumSystem::evalF(state);
	for (unsigned j=1;j<output.size();j+=2) {
			output[j] += m_forceField / mass;
	}
	return output;
}

///TODO: render the system (ie draw the particles)
void ClothSystem::draw()
{	
if (!mesh) {
	Vector3f p1, p2, p3, v1, v2, n;
	for (unsigned x=0;x<m_length;x++) {
		for (unsigned z=0;z<m_width;z++) {
			if (x>0 && z>0) {
				p1 = getParticle(x,z);
				p2 = getParticle(x-1,z);
				p3 = getParticle(x-1,z-1);
				v1 = p2-p1;
				v2 = p3-p1;
				n = Vector3f::cross(v1,v2).normalized();

				glBegin(GL_TRIANGLES);
				glNormal3f(n[0],n[1],n[2]);
				glVertex3f(p1[0],p1[1],p1[2]);
				glNormal3f(n[0],n[1],n[2]);
				glVertex3f(p2[0],p2[1],p2[2]);
				glNormal3f(n[0],n[1],n[2]);
				glVertex3f(p3[0],p3[1],p3[2]);

				p1 = getParticle(x,z);
				p2 = getParticle(x-1,z-1);
				p3 = getParticle(x,z-1);
				v1 = p2-p1;
				v2 = p3-p1;
				n = Vector3f::cross(v1,v2).normalized();

				glNormal3f(n[0],n[1],n[2]);
				glVertex3f(p1[0],p1[1],p1[2]);
				glNormal3f(n[0],n[1],n[2]);
				glVertex3f(p2[0],p2[1],p2[2]);
				glNormal3f(n[0],n[1],n[2]);
				glVertex3f(p3[0],p3[1],p3[2]);

				glEnd();
			}
		}
	}
} else {
	//Mesh Rendering
	glDisable(GL_LIGHTING);
	//PendulumSystem::draw();
	for (unsigned i=0;i<m_springs.size();i++) {
		Spring s = m_springs[i];
		Vector3f p1 = m_vVecState[s.getParticle1() * 2];
		Vector3f p2 = m_vVecState[s.getParticle2() * 2];

		int type = s.getType();
		if (type == 0) {
			glColor3f(255.0,0.0,0.0);
		} else if (type == 1) {
			glColor3f(0.0,255.0,0.0);
		} else if (type == 2) {
			glColor3f(0.0,0.0,255.0);
		} else {
			glColor3f(128.0,128.0,128.0);
		}
		
		glBegin(GL_LINES);
		glLineWidth(5.0); 
		glVertex3f(p1[0],p1[1],p1[2]);
		glVertex3f(p2[0],p2[1],p2[2]);
		glEnd();
		
	}
	glEnable(GL_LIGHTING);
}
}


