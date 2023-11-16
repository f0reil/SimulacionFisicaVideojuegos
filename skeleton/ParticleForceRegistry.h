#pragma once

#include <map>
#include "ForceGenerator.h"

typedef std::pair<ForceGenerator*, Particle*> FRPair;
// Orden del mapa cambiado para mejora de coste del erase de la particula
class ParticleForceRegistry: public std::multimap<Particle*, ForceGenerator*>
{
public:
	void updateForces(double duration);
	void addRegistry(ForceGenerator* fg, Particle* p);
	void deleteParticleRegistry(Particle* p);
	void deleteForceGeneratorRegistry(ForceGenerator* p);
};

