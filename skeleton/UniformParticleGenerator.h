#pragma once
#include "ParticleGenerator.h"
#include <chrono>
class UniformParticleGenerator : public ParticleGenerator
{
public:
	UniformParticleGenerator(std::string name, Vector3 mPos, Vector3 dPos, Vector3 mVel, Vector3 dVel, bool input = false) 
		: ParticleGenerator(name, mPos, mVel, input), gen(std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()))
	{
		_vel_width = dVel / 2;
		_pos_width = dPos / 2;

		posX = new std::uniform_real_distribution<float>(-_pos_width.x, _pos_width.x);
		posY = new std::uniform_real_distribution<float>(-_pos_width.y, _pos_width.y);
		posZ = new std::uniform_real_distribution<float>(-_pos_width.z, _pos_width.z);

		velX = new std::uniform_real_distribution<float>(-_vel_width.x, _vel_width.x);
		velY = new std::uniform_real_distribution<float>(-_vel_width.y, _vel_width.y);
		velZ = new std::uniform_real_distribution<float>(-_vel_width.z, _vel_width.z);
	}
	~UniformParticleGenerator() {}

	virtual std::list<Particle*> generateParticles();
private:
	Vector3 _vel_width, _pos_width;

	std::default_random_engine gen;

	std::uniform_real_distribution<float>* posX;
	std::uniform_real_distribution<float>* posY;
	std::uniform_real_distribution<float>* posZ;

	std::uniform_real_distribution<float>* velX;
	std::uniform_real_distribution<float>* velY;
	std::uniform_real_distribution<float>* velZ;

};

