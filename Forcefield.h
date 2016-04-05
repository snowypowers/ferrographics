#ifndef FORCEFIELD_H
#define FORCEFIELD_H

#include <vecmath.h>
#include <vector>

class ForceField {
public:
	ForceField(Vector3f direction, float mag);

	vector<Vector3f> evalF(vector<Vector3f> state);

protected:
	Vector3f m_dir;

	float m_mag;
}

#endif
