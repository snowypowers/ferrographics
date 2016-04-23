#include "TimeStepper.hpp"

//Takes in a state and applies the force for 1 step
vector<Vector3f> stepForward(vector<Vector3f>& state, vector<Vector3f>& force, float step)
{
	vector<Vector3f> output = vector<Vector3f>();
	Vector3f v;
	for (unsigned i=0;i<state.size();i++) {
		v = state[i] + (force[i] * step);
		output.push_back(v);
	}
	return output;
}
///TODO: implement Explicit Euler time integrator here
void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	
	vector<Vector3f> currentX = particleSystem->getState();
	vector<Vector3f> fx = particleSystem->evalF(currentX);

	vector<Vector3f> newX = stepForward(currentX, fx, stepSize);
	particleSystem->setState(newX);
	particleSystem->checkCollision();
}

void LeapFrog::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	
	vector<Vector3f> currentX = particleSystem->getState();
	vector<Vector3f> fx = particleSystem->evalF(currentX);

	vector<Vector3f> newX = stepForward(currentX, fx, stepSize);
	particleSystem->setState(newX);
	particleSystem->checkCollision();
}

///TODO: implement Trapzoidal rule here
void Trapzoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	vector<Vector3f> currentX = particleSystem->getState();
	vector<Vector3f> f0 = particleSystem->evalF(currentX);
	vector<Vector3f> newX = vector<Vector3f>();
	Vector3f v;

	//initialise x1
	vector<Vector3f> x1 = stepForward(currentX, f0, stepSize);
	vector<Vector3f> f1 = particleSystem->evalF(x1);

	//Construct newX = CurrentX + h/2 * (f0 + f1)
	for (unsigned j=0;j<currentX.size();j++) {
		v = currentX[j] + stepSize * 0.5 * (f0[j] + f1[j]);
		newX.push_back(v);
	}
	particleSystem->setState(newX);
}

void RK4::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	vector<Vector3f> k1, k2, k3, k4, x1, x2, x3;
	vector<Vector3f> currentX = particleSystem->getState();
	k1 = particleSystem->evalF(currentX);

	x1 = stepForward(particleSystem->getState(), k1, stepSize/2.0);
	k2 = particleSystem->evalF(x1);

	x2 = stepForward(particleSystem->getState(), k2, stepSize/2.0);
	k3 = particleSystem->evalF(x2);

	x3 = stepForward(particleSystem->getState(), k3, stepSize);
	k4 = particleSystem->evalF(x3);

	printf("sum all k\n");
	for (unsigned i=0;i<k1.size();i++) {
		currentX[i] += (stepSize / 6.0) * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]);
	}
	printf("cleared\n");
	particleSystem->setState(currentX);
	printf("SET!\n");
	particleSystem->checkCollision();
	printf("END ONE TIME STEP\n");

}
