#include "GravityForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(const Vector3& g)
{
	setGravity(g);
}

void GravityForceGenerator::updateForce(Particle* particle, double t)
{
	// Chekeamos masa finita
	if (fabs(particle->getInvMass()) < 1e-10) return;
	
	particle->addForce(_gravity * particle->getMass());
}
