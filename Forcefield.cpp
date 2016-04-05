#include "Forcefield.h"


ForceField::ForceField(Vector3f dir, float mag) {
	m_dir = dir;
	m_mag = mag;
}

vector<Vector3f> ForceField::evalF(vector<Vector3f> state) {

}