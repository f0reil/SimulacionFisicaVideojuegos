#include "ParticleForceRegistry.h"
#include <iostream>
void ParticleForceRegistry::updateForces(double duration)
{
	for (auto it = begin(); it != end(); ++it)
	{
		it->second->updateForce(it->first, duration);
	}
}

void ParticleForceRegistry::addRegistry(ForceGenerator* fg, Particle* p)
{
	emplace(p,fg);
}

void ParticleForceRegistry::deleteParticleRegistry(Particle* p)
{
	erase(p);
}

void ParticleForceRegistry::deleteForceGeneratorRegistry(ForceGenerator* p)
{
	for (auto it = begin(); it != end();)
	{
		if (it->second == p) it = erase(it);
		else ++it;
	}
}
