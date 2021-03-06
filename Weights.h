#ifndef WEIGHTS_H
#define WEIGHTS_H

class Weights {

public:
	static float default(Vector3f r, float h) {
		if (r.abs() > h) {return 0.0f;}
		float coeff = 315.0f / (64.0f * PI * pow(h, 9.0f));
		return coeff * pow( h*h - r.absSquared(),3.0f);
	};

	static Vector3f default1(Vector3f r, float h) {
		if (r.abs() > h) {return Vector3f(0.0f);}
		return -(945.0f / (32.0  * PI * pow(h, 9.0f)) ) * pow((h*h - r.absSquared()), 2.0f) * r;
	};
	
	static float default2(Vector3f r, float h) {
		if (r.abs() > h) {return 0.0f;}
		return -(945.0f / (32.0  * PI * pow(h, 9.0f)) ) * (h*h - r.absSquared()) * (3.0*h*h - 7 * r.absSquared());
	};

	static Vector3f pressure(Vector3f r, float h) {
		if (r.abs() > h) {return Vector3f(0.0f);}
		if (r.abs() == 0) {return Vector3f(0.001,0.001,0.001);}
		return (-45 / (PI * pow(h,6.0f))) * (r / r.abs()) * pow( h - r.abs() ,2.0f); //KELAGER
		//return (-45.0f / (PI * pow(h,6.0f))) * (r / r.abs()) * pow( h - r.abs() ,3.0f); //HARADA
	};

	static float vis(Vector3f r, float h) {
		if (r.abs() > h) {return 0.0f;}
		return (45.0f / (PI * pow(h, 6.0f))) * (h - r.abs());
	};

};

#endif