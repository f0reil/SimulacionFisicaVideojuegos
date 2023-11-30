#include "WhirlwindForceGenerator.h"

void WhirlwindForceGenerator::updateForce(Particle* p, double t)
{
	Vector3 posParticle = p->getPos().p;
	Vector3 aux = {-(posParticle.z - origins.z)/2, (posParticle.y - origins.y)/5, (posParticle.x - origins.x) / 2 };
	windVel = k * aux;
	ParticleDragGenerator::updateForce(p, t);
}
