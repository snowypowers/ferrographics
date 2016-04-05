#ifndef BOX_H
#define BOX_H

#include <vecmath.h>

class Box
{
/*
Box implementation
This is the hollow square box.

@params
float l: length of side
Vector3f c: Center of the box
Vector3f up: Upward direction of the box
bool top: True for closed box, False for box with open top

*/
public:
	//Default Constructor
	Box();

	//Constructor
	Box(float side, Vector3f c, Vector3f up, bool top, Matrix4f rot) {
	
	
	}

protected:
	float m_side;
	Vector3f m_c;
	Vector3f m_up;
	bool m_top;
	Matrix4f m_rot;











}
#endif