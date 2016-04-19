#ifndef WEIGHTS_H
#define WEIGHTS_H

class Weights {

public:
	static float default(Vector3f r, float h) {
		float coeff = 315 / (64 * PI * pow(h, 9.0f));
		float l = r.abs();
		if (l > h) {return 0.0f;}
		return coeff * pow( h*h - r.absSquared(),3);
	};

	static Vector3f pressure(Vector3f r, float h) {
		if (r.abs() > h) {return Vector3f();}
		return (-45 / (PI * pow(h,6))) * (r / r.abs()) * pow( h - r.abs() ,2);
	};

	static float vis(Vector3f r, float h) {
		if (r.abs() > h) {return 0.0f;}
		return (45 / (PI * pow(h, 6))) * (h - r.abs());
	};

};
#endif