#pragma once
#include "Particle.h"
#include "ParticleGenerator.h"
#include "GaussianParticleGenerator.h"
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

	void update(double t, std::list<Entity*>& _bulletParticlesList, int& score);
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
			for (Entity* p : _particles)
			{
				_particle_force_registry->addRegistry(f, p);
			}
		}
	}
	inline void addForceGeneratorToSpecificParticles(ForceGenerator* f, std::list<Entity*>& listP)
	{
		_force_generators.push_back(f);

		for (Entity* p : listP)
		{
			_particle_force_registry->addRegistry(f, p);
		}
	}
	inline void addParticlesToRegistry(std::list<Entity*>& listP)
	{
		for (ForceGenerator* f : _force_generators)
		{
			for (Entity* p : listP)
			{
				_particle_force_registry->addRegistry(f, p);
			}
		}
	}
	inline void addExplosion()
	{
		explosionForceGenerator = new ExplosionForceGenerator(Vector3(0,-20,-100), 500, 1000000, 20, 2);
		addForceGenerator(explosionForceGenerator);
		for (Entity* p : _particles)
		{
			_particle_force_registry->addRegistry(explosionForceGenerator, p);
		}
	}
	void generateSpringForce();
	void generateSpringForceRigidSolid(physx::PxPhysics* gPhysics, physx::PxScene* gScene);
	void generateAnchoredForce();
	//void generateBuoyancyForce();

	void generateBuoyancyForce(physx::PxPhysics* gPhysics, physx::PxScene* gScene);
	inline void addBouyancyForces(BouyancyForceGenerator* b, Entity* e)
	{
		_bouyancy_force_generators.push_back(b);
		_bouyancy_particles.push_back(e);
	}

	inline SpringForceGenerator* getSprinFG_1() { return f1; };
	inline SpringForceGenerator* getSprinFG_2() { return f2; };

	inline void addParticleToList(Entity* p, bool bullet = false) 
	{ 
		_particles.push_back(p);
		if (bullet)
		{
			_bulletParticles.push_back(p);
		}
	};

	void deleteLists();
	void deleteParticles();

	inline bool getActive() { return active; };
	inline void setActive(bool a) { active = a; };

	// Colisiones
	void checkColissionsWithBullets(int& score, physx::PxActor* actor1, physx::PxActor* actor2, std::list<Entity*>& _bulletParticlesList);
	inline void updateScore(int& score)
	{
		++score;
	}

	void createFireWork();

	inline void Fireworks()
	{
		createFireWork();
		shootFirework();
	}

private:
	std::list<Entity*> _particles;
	std::list<Entity*> _bulletParticles;
	std::list<ParticleGenerator*> _particles_generator;
	std::unordered_map<std::string, ParticleGenerator*> _pGenerator_map;

	std::list<ForceGenerator*> _force_generators;
	
	ParticleForceRegistry* _particle_force_registry;
	ExplosionForceGenerator* explosionForceGenerator = nullptr;

	// Generador Gaussiano para firework
	GaussianParticleGenerator* gausFireworkGenerator = nullptr;

	SpringForceGenerator* f1; 
	SpringForceGenerator* f2;

	std::list<BouyancyForceGenerator*> _bouyancy_force_generators;
	std::list<Entity*> _bouyancy_particles;

	bool active = true;
};

