#pragma once
#include "ParticleGenerator.h"
class GaussianParticleGenerator: public ParticleGenerator
{
public:
	GaussianParticleGenerator(std::string name, Vector3 mPos, Vector3 mVel, Vector3 varianza, int numP = 100, bool input = false, float time = 0.3)
								: ParticleGenerator(name, mPos, mVel, numP,input, time), gen(std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()))
	{
		velX = new std::normal_distribution<float>(mVel.x, varianza.x);
		velY = new std::normal_distribution<float>(mVel.y, varianza.y);
		velZ = new std::normal_distribution<float>(mVel.z, varianza.z);
	}
	~GaussianParticleGenerator() {}

	virtual std::list<Entity*> generateParticles();
private:
	std::default_random_engine gen;

	std::normal_distribution<float>* velX;
	std::normal_distribution<float>* velY;
	std::normal_distribution<float>* velZ;

};

