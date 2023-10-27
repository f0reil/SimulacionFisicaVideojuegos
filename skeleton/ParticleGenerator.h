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
	ParticleGenerator(std::string name, Vector3 mPos, Vector3 mVel, int numP = 3, bool input = false) 
		: _name(name), _mean_pos(mPos), _mean_vel(mVel), num_particles(numP), _input(input) {}
	~ParticleGenerator() {}

	inline void setMeanVelocity(const Vector3& v) {
		_mean_vel = v;
	}
	inline Vector3 getMeanVelocity() const {
		return _mean_vel;
	}
	void setParticle(Particle* p, bool modify_pos_vel = true);
	inline void setNParticles(int n_p) { num_particles = n_p; }
	virtual std::list<Particle*> generateParticles() = 0;
	inline void setPos(Vector3 p) { _mean_pos = p; };
	inline std::string getGeneratorName() { return _name; }
	inline bool getInput() { return _input; }
protected:
	std::string _name;
	Vector3 _mean_pos, _mean_vel;
	int num_particles = 3;
	Particle* _model = nullptr; 
	bool _input;

};

