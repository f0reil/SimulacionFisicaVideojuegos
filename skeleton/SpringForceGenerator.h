#pragma once
#include "ForceGenerator.h"
#include "core.hpp"

class SpringForceGenerator: public ForceGenerator
{
public:
	SpringForceGenerator(double k, double resting_lenght, Particle* other);

	virtual void updateForce(Particle* particle, double t);

	inline void setK(double k) { _k = k; };

	~SpringForceGenerator() {};
protected:
	double _k;
	double _resting_lenght;
	Particle* _other = nullptr;
};

