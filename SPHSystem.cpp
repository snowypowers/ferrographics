#include "SPHSystem.h"

SPHSystem::SPHSystem() :ParticleSystem(){
	srand (0);
	m = new Water(1000, 0.01, 500, 0.0457); 
	m_numParticles = 500;
	//m_vVecState = vector<Vector3f>(200);
	for (int i=0;i<500;i++) {
		m_vVecState.push_back(Vector3f((float)( rand() % 10000)/10000.0,(float)(rand() % 10000)/10000.0, (float)(rand() % 10000)/10000.0));
		m_vVecState.push_back(Vector3f((float)(rand()%10)/10 , (float)(rand()%10)/10  , (float)(rand()%10)/10 ));
	}
	hash = new SpatialHash(100, 0.0457);
	box = Box(1, Vector3f(0.5,0.5,0.5), Vector3f(0,1,0), true);
};

SPHSystem::SPHSystem(Material* mat, int numParticles, bool empty):ParticleSystem() {
	m = mat;
	m_numParticles = numParticles;
	if (!empty) {
		for (int i=0;i<(m_numParticles*2);i++) {
			if (i%2 == 0) {
				m_vVecState.push_back(Vector3f(i/100,i/100,i/100));
			} else {
				m_vVecState.push_back(Vector3f());
			}
		}
	}
	hash = new SpatialHash(numParticles, 0.01);
	//Create box boundary
	box = Box();
}

void SPHSystem::addParticle(Vector3f pos, Vector3f velo) {
	m_vVecState.push_back(pos);
	m_vVecState.push_back(velo);
};

vector<Vector3f> SPHSystem::evalF(vector<Vector3f> state) {
	//printf("Start eval\n");
	int p, i;//Counter
	hash->clear();
	vector<Vector3f> output = vector<Vector3f>();
	//printf("Cleared Hash\n");
	//Insert all particles into hash
	//hash->insert(state);
	//hash->overview();
	//printf("Inserted Hash\n");

	vector<vector<int>> neighboursArray = vector<vector<int>> ();
	vector<float> mass_densityArray = vector<float>();
	vector<float> pressureArray = vector<float>();
	vector<int> neighbours;
	float mass_density, pressure;
	//Calculate density and pressure for each particle
	for (i=0;i<m_numParticles;++i) {
			//printf("Start particle %d\n", i);
		mass_density = m->getMass();
		pressure = 0;
		//find all possible neighbours
			//printf("Find neighbours  ");
		//neighbours = hash->findNeighbours(state[i*2], m->getH());

		neighbours = vector<int>();
		for (int j=0;j<m_numParticles;++j) {
			//printf("%d %d\n",i,j);
			if (i == j) {continue;}
			if ((state[i*2] - state[j*2]).abs() < m->getH()) {
				printf("Neighbour dist: %f\n", (state[i*2] - state[j*2]).abs());
				neighbours.push_back(j);
			}
		}
			//printf("No. of neighbours: %d  ", neighbours.size());
		for (int j=0;j<neighbours.size();j++) {
			//printf("get particle %d %d \n ",neighbours[j]*2, j);
			Vector3f pos = state[neighbours[j]*2];
				
				//pos.print();
			//If neighbour particle is within support radius of particle i
			if ((pos - state[i*2]).abs() <= m->getH()) {
				Vector3f diff = state[i*2] - pos;
				mass_density += m->getMass() * Weights::default(diff,m->getH());
			} else {
			//Set invalid neighbour to -1
				neighbours.erase(neighbours.begin() + j);
				//neighbours[i] = -1;
			}
		}
		//printf("end neighbours\n");
		//Calculate pressure using rest pressure
		pressure = m ->getK() * ( mass_density - m->getRestPressure());
		printf ("%d: neighbours: %d | mass_density: %f | pressure: %f\n", i, neighbours.size(), mass_density, pressure);
		//printf("Neighbours: %d\n", neighbours.size());
		//printf("Neighbours: %d\n", neighboursArray.size());
		//printf("%d\n", neighboursArray.max_size());
		neighboursArray.push_back(neighbours);
		//printf("pushed neighbours\n");
		mass_densityArray.push_back(mass_density);
		//printf("pushed massDens\n");
		pressureArray.push_back(pressure);
		//printf("pushed pressure\n");
	}
	printf("find forces\n");
	for (i=0;i<m_numParticles;i++) {
		//neighbours = hash->findNeighbours(i, 0.0457);
		//compute internal forces for each particle
		vector<int> neighbours = neighboursArray[i];
		Vector3f pressureForce = Vector3f();
		Vector3f visForce = Vector3f();
		float pi = pressureArray[i];
		float mi = mass_densityArray[i];
		Vector3f vi = state[i*2 + 1];

		//pi.print();
		//mi.print();
		//vi.print();
		for (int j=0;j<neighbours.size();j++) {
			int index = neighbours[j];
			if (index == -1) {continue;}
			if (index == i) {continue;}
			Vector3f diff = state[i*2] - state[index*2];
			float pj = pressureArray[index];
			float mj = mass_densityArray[index];
			pressureForce -= ( (pi + pj) / 2 ) * (mj / pj) * Weights::pressure(diff, m->getH());
			visForce += (state[index*2 + 1] - vi) * (mj / pj) * Weights::vis(diff, m->getH());
		}
		visForce  = visForce * m->getViscosity();
		//External forces
		Vector3f gravityForce = Vector3f(0, mi * -9.81, 0);

		//Sum up all forces and put into output
		//Push in velocity
		output.push_back(state[i*2 + 1]);
		Vector3f finalForce = (pressureForce + visForce + gravityForce) / mi;
		printf("%d:\n", i);
		state[i*2].print();
		finalForce.print();
		visForce.print();
		pressureForce.print();
		gravityForce.print();
		output.push_back(finalForce);
	}
	printf("end eval");
	return output;

	
};

void SPHSystem::checkCollision(){
	float wallDamping = -0.1f;
	Vector3f pos, vel;
	vector<Vector3f> newState;
	float* points = new float[6];
	box.getPoints(points);
	printf("%f %f %f %f %f %f", points[0], points[1], points[2], points[3], points[4], points[5]);
	for (int n=0; n<m_vVecState.size();n=n+2){
		 pos = m_vVecState[n];
		 vel = m_vVecState[n+1];
		 if(box.collide(pos)){
			 if(pos.x()<points[0]){
				 vel[0] = vel[0] * wallDamping;
				 pos[0] = points[0];
			 } else if (pos.x()>points[1]){
				 vel[0] = vel[0] * wallDamping;
				 pos[0] = points[1];
			 } 
			 if (pos.y()>points[2]){
				 vel[1] = vel[1] * wallDamping;
				 pos[1] = points[2];
			 } else if (pos.y()<points[3]){
				 vel[1] = vel[1] * wallDamping;
				 pos[1] = points[3];
			 } 
			 if (pos.z()<points[4]){
				 vel[2] = vel[2] * wallDamping;
				 pos[2] = points[4];
			 } else if (pos.z()>points[5]){
				 vel[2] = vel[2] * wallDamping;
				 pos[2] = points[5];
			 } 
		 }
		 newState.push_back(pos);
		 newState.push_back(vel);
	}
	delete[] points;
	this->setState(newState);
}

void SPHSystem::draw() {
	float r = m->getRadius();
	for (int i=0;i<m_vVecState.size();i+=2) {
		Vector3f pos = m_vVecState[i];
		Vector3f vel = m_vVecState[i+1];
		Vector3f col;
		if(vel.abs() > 10) { col = Vector3f(1,0,0);}
		else if (vel.abs() > 5) {col = Vector3f(0.5,0,0);}
		else {col = Vector3f(0,0,0.5);}
		glPushMatrix();
		glColor3f(col[0], col[1], col[2]);
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(r,5.0f,5.0f);
		glPopMatrix();
	}
};

