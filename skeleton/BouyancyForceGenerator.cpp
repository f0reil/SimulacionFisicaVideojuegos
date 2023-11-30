#include "BouyancyForceGenerator.h"

void BouyancyForceGenerator::updateForce(Particle* particle, double t)
{
	float h = particle->getPos().p.y;
	float h0 = _liquid_particle->getPos().p.y;

	Vector3 f(0,0,0);
	float immersed = 0.0;
	if (h - h0 > _height * 0.5)
	{
		immersed = 0.0;
	}
	else if (h0 - h > _height * 0.5)
	{
		immersed = 1.0;
	}
	else
	{
		immersed = (h0 - h) / _height + 0.5;
	}
	f.y = _liquid_density * _volumen * immersed * 9.8;

	particle->addForce(f);
}

BouyancyForceGenerator::~BouyancyForceGenerator()
{
	delete _liquid_particle;
}
