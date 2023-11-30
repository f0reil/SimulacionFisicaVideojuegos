#pragma once
#include "ForceGenerator.h"
#include "core.hpp"

class BouyancyForceGenerator: public ForceGenerator
{
public: 
	BouyancyForceGenerator(float h, float v, float d, Particle* l) :
		ForceGenerator(), _height(h), _volumen(v), _liquid_density(d), _liquid_particle(l) {};
	virtual void updateForce(Particle* particle, double t);

	virtual ~BouyancyForceGenerator();

protected:
	float _height;
	float _volumen;
	float _liquid_density;
	float _gravity = 9.8;

	Particle* _liquid_particle;
};

