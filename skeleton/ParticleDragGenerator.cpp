#include "ParticleDragGenerator.h"

ParticleDragGenerator::ParticleDragGenerator()
{
}

ParticleDragGenerator::ParticleDragGenerator(Vector3 wVel, Vector3 pOrigin, Vector3 limits, const float k1, const float k2)
{
	windVel = wVel;
	origins = pOrigin;
	limit = limits;
	setDrag(k1, k2);
}

void ParticleDragGenerator::updateForce(Entity* p, double t)
{
	if (fabs(p->getInvMass() < 1e-10)) return;

	if(p->insideLimits(origins, limit))
	{
		Vector3 aux = windVel - p->getVel();
		Vector3 v = _k1 * aux + _k2 * aux.magnitude() * aux;
		p->addForce(v);
	}
}
