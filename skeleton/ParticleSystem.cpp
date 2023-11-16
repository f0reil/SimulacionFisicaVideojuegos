#include "ParticleSystem.h"
#include <iostream>

void ParticleSystem::update(double t)
{
	
	for (auto it = _particles.begin(); it != _particles.end();)
	{
		Particle* p = static_cast<Particle*>(*it);  
		// Muere por tiempo o sale de los limites
		if (p->getTimeLeft() > 0 && !p->insideLimits(Vector3(10000))) 
		{
			p->integrate(t);
			++it;
		}
		else
		{
			p->death();
			if (p->getGeneratesOnDeath())
			{
				_particles.splice(_particles.end(), p->getGeneratedParticles());
			}
			_particle_force_registry->deleteParticleRegistry(p);
			delete(*it);
			*it = nullptr;
			it = _particles.erase(it);
		}
	}
	// Recorremos generadores para añadir la lista de particulas generadas
	for (ParticleGenerator* g: _particles_generator)
	{
		if (!g->getInput())
		{
			std::list<Particle*> listP = g->generateParticles();
			addParticlesToRegistry(listP);
			_particles.splice(_particles.end(), listP);
		}
	}
	for (auto it = _force_generators.begin(); it != _force_generators.end();)
	{
		(*it)->updateTime(t);
		// En caso de que se pase la duración, se borra el generador de fuerzas
		if (!(*it)->checkTime())
		{
			_particle_force_registry->deleteForceGeneratorRegistry((*it));
			auto aux = (*it);
			it = _force_generators.erase(it);
			delete(aux);
		}
		else ++it;	
	}
	_particle_force_registry->updateForces(t);
	
}

ParticleSystem::~ParticleSystem()
{
	for (auto it = _particles.begin(); it != _particles.end();)
	{
		delete(*it);
		*it = nullptr;
		it = _particles.erase(it);
	}
	for (auto it = _particles_generator.begin(); it != _particles_generator.end(); )
	{
		delete(*it);
		*it = nullptr;
		it = _particles_generator.erase(it);
	}
	for (auto it = _force_generators.begin(); it != _force_generators.end(); )
	{
		delete(*it);
		*it = nullptr;
		it = _force_generators.erase(it);
	}
}
