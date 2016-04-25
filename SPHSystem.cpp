#include "SPHSystem.h"

SPHSystem::SPHSystem() :ParticleSystem(){
	srand (0);
	m_numParticles = 2000;
	m = new Water(1000.0, 0.04, m_numParticles, 0.05); 

	/*for (int i=0;i<500;i++) {
		//m_vVecState.push_back(Vector3f(i/250.0, Weights::vis(Vector3f(i/250.0),1),0.0));
		//m_vVecState.push_back(Vector3f());
		//m_vVecState.push_back(Vector3f((float)( rand() % 10000)/10000.0,(float)(rand() % 10000)/10000.0, (float)(rand() % 10000)/10000.0));
		//m_vVecState.push_back(Vector3f((float)(rand()%100)/100 , (float)(rand()%100)/100  , (float)(rand()%100)/100 ));
	}*/
	//hash = new SpatialHash(100, m->getH());

	for (int i=0;i<10;i++) {
		for (int j=0;j<20;j++) {
			for (int k=0;k<10;k++) {
				m_vVecState.push_back(Vector3f(i/20.0, j/40.0, k/20.0));
				//hash->hash(Vector3f(i/40.0, j/20.0, k/20.0));
				//m_vVecState.push_back(Vector3f());
				m_vVecState.push_back(Vector3f((float)( rand() % 100)/1000.0, -(float)( rand() % 100)/1000.0, (float)( rand() % 100)/1000.0));
			}
		}
	}
	printf("Properties: \nMass %f\n", m->getMass());
	printf("Support: %f\n", m->getSupport());
	
	box = Box(0.5, Vector3f(0.25,0.25,0.25), Vector3f(0,1,0), true);
	box.initTable(m->getH());
	fsphere = ForceSphere(Vector3f(0,0,0),0.5, 0.2,3);
	
	//Intialise Bins, Cell Size = h*2
	float* points = new float[6];
	box.getPoints(points);
	cellsPerSide  =(int) ceil(box.getSide()/m->getH()*2);
	bins = vector<vector<int>>();
	for(int i=0; i<cellsPerSide*cellsPerSide*cellsPerSide;i++){
		bins.push_back(vector<int>());
	}
	
};



SPHSystem::SPHSystem(Material* mat, int numParticles, bool empty):ParticleSystem() {
	m = mat;
	m_numParticles = numParticles;
	hash = new SpatialHash(numParticles, 0.01);
	if (!empty) {
		for (int i=0;i<(m_numParticles*2);i++) {
			if (i%2 == 0) {
				m_vVecState.push_back(Vector3f(i/100,i/100,i/100));
			} else {
				m_vVecState.push_back(Vector3f());
			}
		}
	}

	//Create box boundary
	box = Box();
	fsphere = ForceSphere();
}

void SPHSystem::addParticle(Vector3f pos, Vector3f velo) {
	m_vVecState.push_back(pos);
	m_vVecState.push_back(velo);
};

vector<Vector3f> SPHSystem::evalF(vector<Vector3f> state) {
	//printf("Start eval\n");
	int p, i;//Counter
	box.clear();
	vector<Vector3f> output = vector<Vector3f>();
	//printf("Cleared Hash\n");
	//Insert all particles into hash
	box.insert(state);
	//printf("Inserted Hash\n");

	vector<vector<int>> neighboursArray = vector<vector<int>> ();
	vector<float> mass_densityArray = vector<float>();
	vector<float> pressureArray = vector<float>();
	vector<int> neighbours;
	float mass_density, pressure;
	//Calculate density and pressure for each particle
	for (i=0;i<m_numParticles;++i) {
			//printf("Start particle %d\n", i);
		mass_density = m->getMass() * Weights::default(Vector3f(),m->getH());
		pressure = 0;
		//find all possible neighbours
			//printf("Find neighbours  ");
		neighbours = box.findNeighbours(state[i*2]);
		/*
		neighbours = vector<int>();
		for (int j=0;j<m_numParticles;++j) {
			//printf("%d %d\n",i,j);
			if (i == j) {continue;}
			if ((state[i*2] - state[j*2]).abs() <= m->getSupport()) {
				//printf("Neighbour dist: %f\n", (state[i*2] - state[j*2]).abs());
				neighbours.push_back(j);
			}
		}*/

			//printf("No. of neighbours: %d  ", neighbours.size());
		for (int j=0;j<neighbours.size();j++) {
			//printf("get particle %d %d \n ",neighbours[j]*2, j);
			Vector3f pos = state[neighbours[j]*2];
			//pos.print();
			//If neighbour particle is within H of particle i
			if ((state[i*2] - pos).absSquared() < m->getH()*m->getH()) {
				Vector3f diff =  state[i*2] - pos;
				mass_density += m->getMass() * Weights::default(diff,m->getH());
			}
		}
		//printf("end neighbours\n");
		//Calculate pressure using rest pressure
		pressure = m ->getK() * ( mass_density - m->getRestPressure());
		//printf ("%d: neighbours: %d | mass_density: %f | pressure: %f\n", i, neighbours.size(), mass_density, pressure);
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
	//printf("find forces\n");
	for (i=0;i<m_numParticles;i++) {
		//neighbours = hash->findNeighbours(i, 0.0457);
		//compute internal forces for each particle
		vector<int> neighbours = neighboursArray[i];
		Vector3f pressureForce = Vector3f();
		Vector3f visForce = Vector3f();
		float pi = pressureArray[i];
		float mi = mass_densityArray[i];
		float color = (m->getMass() / mi) * Weights::default2(Vector3f(), m->getH());;
		Vector3f vi = state[i*2 + 1];
		Vector3f normal = (m->getMass() / mi) * Weights::default1(Vector3f(), m->getH());
		Vector3f surfaceTension = Vector3f();

		//pi.print();
		//mi.print();
		//vi.print();
		for (int j=0;j<neighbours.size();j++) {
			int index = neighbours[j];
			//if (index == -1) {continue;}
			Vector3f diff = state[i*2] - state[index*2];
			float pj = pressureArray[index];
			float mj = mass_densityArray[index];
			if (diff.abs() < m->getH()) {
				pressureForce -= ( (pi + pj) / 2 ) * (m->getMass() / mj) * Weights::pressure(diff, m->getH());
				visForce += (state[index*2 + 1] - vi) * (m->getMass() / mj) * Weights::vis(diff, m->getH());
				color += (m->getMass() / mj) * Weights::default2(diff, m->getH());
				normal += (m->getMass() / mj) * Weights::default1(diff, m->getH());
			}
		}
		visForce  = visForce * m->getViscosity();
		if (normal.abs() > 7.0f) {
			surfaceTension = normal.normalized() * -1.0f * color * m->getSurfaceTension();
		}
		//External forces
		Vector3f gravityForce = Vector3f(0, mi * -9.81, 0);
		Vector3f magneticForce = Vector3f();
		Vector3f pos = state[i*2];
		if(this->getForceSphere()->intersect(pos)){
			//magneticForce+=this->getForceSphere()->polarize(pos);
		}
		//Push in velocity
		output.push_back(state[i*2 + 1]);
		//Sum up all forces and put into output
		Vector3f finalForce = (surfaceTension + pressureForce + visForce + gravityForce + magneticForce) / mi;
		/*printf("%d: neighbours: %d\n", i, neighbours.size());
		state[i*2].print();
		state[i*2 +1].print();
		printf("FORCES:\n");
		visForce.print();
		pressureForce.print();
		gravityForce.print();
		magneticForce.print();
		finalForce.print();*/
		output.push_back(finalForce);
	}
	return output;

	
};

void SPHSystem::checkCollision(){
	float wallDamping = -0.5f;
	Vector3f pos, vel;
	vector<Vector3f> newState;
	float* points = new float[6];
	box.getPoints(points);
	//printf("%f %f %f %f %f %f", points[0], points[1], points[2], points[3], points[4], points[5]);
	for (int n=0; n<m_vVecState.size();n=n+2){
		 pos = m_vVecState[n];
		 vel = m_vVecState[n+1];
			 if(pos.x()<points[0]){
				 vel[0] = vel[0] * wallDamping;
				 pos[0] = points[0] - (pos[0] - points[0]);
			 } else if (pos.x()>points[1]){
				 vel[0] = vel[0] * wallDamping;
				 pos[0] = points[1] + (points[1] - pos[0]);
			 } 
			 if (pos.y()<points[2]){
				 vel[1] = vel[1] * wallDamping;
				 pos[1] = points[2] - (pos[1] - points[2]);
			 } else if (pos.y()>points[3]){
				 vel[1] = vel[1] * wallDamping;
				 pos[1] = points[3] + (points[3] - pos[1]);
			 } 
			 if (pos.z()<points[4]){
				 vel[2] = vel[2] * wallDamping;
				 pos[2] = points[4] - (pos[2] - points[4]);
			 } else if (pos.z()>points[5]){
				 vel[2] = vel[2] * wallDamping;
				 pos[2] = points[5] + (points[5] - pos[2]);
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
		GLfloat col [] = {0.7, 0.7, 0.7, 1.0};
		GLfloat col_in [] = {0.7, 0.7, 0.7, 1.0};
		if (vel.abs() > 10) {col[0] += 0.7;}
		if (this->getForceSphere()->intersect(pos)) {col[1] += 0.7;}
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, col);
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(r,5.0f,5.0f);
		glPopMatrix();
	}
};

