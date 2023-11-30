#pragma once
#include "Particle.h"
#include "ParticleGenerator.h"
#include "ForceGenerator.h"
#include "ParticleForceRegistry.h"
#include "ExplosionForceGenerator.h"
#include "SpringForceGenerator.h"
#include "BouyancyForceGenerator.h"
#include <list>
#include <unordered_map>
class ParticleSystem
{
public:
	ParticleSystem() { _particle_force_registry = new ParticleForceRegistry(); };
	~ParticleSystem();

	void update(double t);
	inline ParticleGenerator* getParticleGenerator(const std::string& n)
	{
		auto it = _pGenerator_map.find(n);
		if (it == _pGenerator_map.end()) throw std::invalid_argument("generator does not exist");
		else return (*it).second;
	};
	inline void addGenerator(ParticleGenerator* g)
	{ 
		_particles_generator.push_back(g);
		_pGenerator_map[g->getGeneratorName()] = g;
	};
	inline void shootFirework()
	{
		_particles.splice(_particles.end(), getParticleGenerator("GaussianFireworkGenerator")->generateParticles());
	}
	inline void addForceGenerator(ForceGenerator* f, bool add = false)
	{
		_force_generators.push_back(f);
		if (add)
		{
			for (Particle* p : _particles)
			{
				_particle_force_registry->addRegistry(f, p);
			}
		}
	}
	inline void addParticlesToRegistry(std::list<Particle*>& listP)
	{
		for (ForceGenerator* f : _force_generators)
		{
			for (Particle* p : listP)
			{
				_particle_force_registry->addRegistry(f, p);
			}
		}
	}
	inline void addExplosion()
	{
		explosionForceGenerator = new ExplosionForceGenerator(Vector3(0,30,0), 50, 10000, 20, 3);
		addForceGenerator(explosionForceGenerator);
		for (Particle* p : _particles)
		{
			_particle_force_registry->addRegistry(explosionForceGenerator, p);
		}
	}
	void generateSpringForce();
	void generateAnchoredForce();
	void generateBuoyancyForce();

	inline SpringForceGenerator* getSprinFG_1() { return f1; };
	inline SpringForceGenerator* getSprinFG_2() { return f2; };
private:
	std::list<Particle*> _particles;
	std::list<ParticleGenerator*> _particles_generator;
	std::unordered_map<std::string, ParticleGenerator*> _pGenerator_map;

	std::list<ForceGenerator*> _force_generators;
	
	ParticleForceRegistry* _particle_force_registry;
	ExplosionForceGenerator* explosionForceGenerator = nullptr;

	SpringForceGenerator* f1; 
	SpringForceGenerator* f2;
};

