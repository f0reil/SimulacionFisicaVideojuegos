#pragma once
#include <string>
#include "core.hpp"
#include "Particle.h"
#include <list>
#include <random>
#include <chrono>

class ParticleGenerator
{
public:
	ParticleGenerator(std::string name, Vector3 mPos, Vector3 mVel, int numP = 100, bool input = false, float time = 0.3) 
		: _name(name), _mean_pos(mPos), _mean_vel(mVel), num_particles(numP), _input(input), maxTime(time) 
	{
		this->time = 0;
		contEnts = 0;
	}
	~ParticleGenerator() {}

	inline void setMeanVelocity(const Vector3& v) {
		_mean_vel = v;
	}
	inline Vector3 getMeanVelocity() const {
		return _mean_vel;
	}
	void setParticle(Entity* p, bool modify_pos_vel = true);
	inline void setNParticles(int n_p) { num_particles = n_p; }
	virtual std::list<Entity*> generateParticles() = 0;
	inline void setPos(Vector3 p) { _mean_pos = p; };
	inline std::string getGeneratorName() { return _name; }
	inline bool getInput() { return _input; }
	inline bool timerGenerator(double t)
	{ 
		time += t; 
		if (time > maxTime) { time = 0; return true; }
		else return false;
	}
	inline bool maxNumEnts()
	{
		// Si el contador no se ha sobrepasado, se generaran entidades
		return contEnts < num_particles;
	}
protected:
	std::string _name;
	Vector3 _mean_pos, _mean_vel;
	int num_particles = 3;
	Entity* _model = nullptr; 
	bool _input;
	float time, maxTime;
	int contEnts;
};

