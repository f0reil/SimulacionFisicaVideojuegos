#include "ExplosionForceGenerator.h"

void ExplosionForceGenerator::updateForce(Entity* particle, double t)
{
	Vector3 pPos = particle->getPos().p;
	Vector3 dir = pPos - _centro;
	float r = dir.magnitude();

	if (r < _radio)
	{
		Vector3 f = _k / pow(r, 2) * dir * exp(-t / _t);
		particle->addForce(f);
	}
}
