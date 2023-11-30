#include "ParticleSystem.h"
#include "AnchoredSpringFG.h"
#include <iostream>

void ParticleSystem::update(double t)
{
	
	for (auto it = _particles.begin(); it != _particles.end();)
	{
		Particle* p = static_cast<Particle*>(*it);  
		// Muere por tiempo o sale de los limites
		if (p->getTimeLeft() > 0 && !p->insideLimits(Vector3(10000000))) 
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

void ParticleSystem::generateSpringForce()
{
	Particle* p1 = new Particle({10.0,50.0,0.0}, {0.0,0.0,0.0}, {0.0,0.0,0.0}, {0.0,0.0,0.0}, 0.85, 0, 0, 60, Sphere, 1, { 255, 255,255,1 });
	Particle* p2 = new Particle({ -10,50,0 }, { 0,0,0,}, { 0,0,0 }, { 0,0,0 }, 0.85, 0, 0, 60, Sphere, 1, { 0, 0,0,1 });
	p2->setMass(2.0);
	p1->setMass(2.0);
	f1 = new SpringForceGenerator(1,10, p2);
	_particle_force_registry->addRegistry(f1, p1);
	f2 = new SpringForceGenerator(1,10,p1);
	_particle_force_registry->addRegistry(f2, p2);
	_force_generators.push_back(f1);
	_force_generators.push_back(f2);
	_particles.push_back(p1);
	_particles.push_back(p2);
	
}

void ParticleSystem::generateAnchoredForce()
{
	Particle* p3 = new Particle({ 30.0, 60.0, 0.0 }, { 0.0,0.0,0.0 }, { 0.0,0.0,0.0 }, { 0.0,0.0,0.0 }, 0.85, 0.0, 0.0, 60, Sphere, 1, { 255, 0,0,1 });
	p3->setMass(2);
	AnchoredSpringFG* f3 = new AnchoredSpringFG(1, 10, { 30.0,55.0,0.0 });
	_particle_force_registry->addRegistry(f3, p3);
	_force_generators.push_back(f3);
	_particles.push_back(p3);
}

void ParticleSystem::generateBuoyancyForce()
{
	Particle* liquid = new Particle({ 30.0, 10.0, 0.0 }, { 0.0,0.0,0.0 }, { 0.0,0.0,0.0 }, { 0.0,0.0,0.0 },
		0.85, 0.0, 0.0, 60, Liquid);
	// PARTICULA 1
	Particle* p = new Particle({ 30.0, 30.0, 0.0 }, { 0.0,0.0,0.0 }, { 0.0,0.0,0.0 }, { 0.0,0.0,0.0 },
		0.98, 0.0, 0.0, 60, Box, 3);
	p->setMass(5);
	BouyancyForceGenerator* bGF = new BouyancyForceGenerator(2, p->getVolumen(), 1000, liquid);

	_particle_force_registry->addRegistry(bGF, p);
	_force_generators.push_back(bGF);
	_particles.push_back(p);
	
	// PARTICULA 2
	Particle* p2 = new Particle({40.0, 20.0, 0.0}, {0.0,0.0,0.0}, {0.0,0.0,0.0}, {0.0,0.0,0.0},
		0.98, 0.0, 0.0, 60, Box, 3, {100,200,200,1});
	p2->setMass(50);

	BouyancyForceGenerator* bGF2 = new BouyancyForceGenerator(5, p2->getVolumen(), 1000, liquid);

	_particle_force_registry->addRegistry(bGF2, p2);
	_force_generators.push_back(bGF2);
	_particles.push_back(p2);

	// PARTICULA 3
	Particle* p3 = new Particle({20.0, 20.0, 0.0}, {0.0,0.0,0.0}, {0.0,0.0,0.0}, {0.0,0.0,0.0},
		0.98, 0.0, 0.0, 60, Box, 1, { 100,0,200,1 });
	p3->setMass(10);

	BouyancyForceGenerator* bGF3 = new BouyancyForceGenerator(5, p3->getVolumen(), 1000, liquid);

	_particle_force_registry->addRegistry(bGF3, p3);
	_force_generators.push_back(bGF3);
	_particles.push_back(p3);
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
