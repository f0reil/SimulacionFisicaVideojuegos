#pragma once

#include <map>
#include "ForceGenerator.h"

typedef std::pair<ForceGenerator*, Entity*> FRPair;
// Orden del mapa cambiado para mejora de coste del erase de la particula
class ParticleForceRegistry: public std::multimap<Entity*, ForceGenerator*>
{
public:
	void updateForces(double duration);
	void addRegistry(ForceGenerator* fg, Entity* p);
	void deleteParticleRegistry(Entity* p);
	void deleteForceGeneratorRegistry(ForceGenerator* p);
};

