#pragma once
#include "ForceGenerator.h"
#include "core.hpp"

class ExplosionForceGenerator: public ForceGenerator
{
public:
	ExplosionForceGenerator(Vector3 centro, int radio, const int k, const int t, double duration) :
		_centro(centro), _radio(radio), _k(k), _t(t) 
	{
		_duration = duration;
	};
	virtual void updateForce(Particle* particle, double t);

protected:
	Vector3 _centro = { 0,0,0 };
	int _radio;
	int _k;
	int _t;
};

