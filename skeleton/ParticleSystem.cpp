#include "ParticleSystem.h"
#include <iostream>

void ParticleSystem::update(double t)
{
	for (auto it = _particles.begin(); it != _particles.end();)
	{
		Particle* p = static_cast<Particle*>(*it);  
		// Muere por tiempo o sale de los limites
		if (p->getTimeLeft() > 0 && !p->insideLimits()) 
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
			_particles.splice(_particles.end(), listP);
		}
	}
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
}
