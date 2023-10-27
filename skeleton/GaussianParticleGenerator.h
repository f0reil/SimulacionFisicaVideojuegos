#pragma once
#include "ParticleGenerator.h"
class GaussianParticleGenerator: public ParticleGenerator
{
public:
	GaussianParticleGenerator(std::string name, Vector3 mPos, Vector3 mVel, Vector3 varianza, int numP = 3, bool input = false) 
								: ParticleGenerator(name, mPos, mVel, numP,input), gen(std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()))
	{
		velX = new std::normal_distribution<float>(mVel.x, varianza.x);
		velY = new std::normal_distribution<float>(mVel.y, varianza.y);
		velZ = new std::normal_distribution<float>(mVel.z, varianza.z);
	}
	~GaussianParticleGenerator() {}

	virtual std::list<Particle*> generateParticles();
private:
	std::default_random_engine gen;

	std::normal_distribution<float>* velX;
	std::normal_distribution<float>* velY;
	std::normal_distribution<float>* velZ;

};

