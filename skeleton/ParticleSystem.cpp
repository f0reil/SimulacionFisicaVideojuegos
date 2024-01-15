#include "ParticleSystem.h"
#include "AnchoredSpringFG.h"
#include "RigidSolid.h"
#include "Firework.h"
#include "ecs.h"
#include <iostream>

void ParticleSystem::update(double t, std::list<Entity*>& _bulletParticlesList, int& score)
{
	
	for (auto it = _particles.begin(); it != _particles.end();)
	{
		Entity* p = static_cast<Entity*>(*it);  
		// Muere por tiempo 
		if (p->getTimeLeft() > 0) 
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
			if ((*it)->getIsProyectil())
			{
				_bulletParticlesList.remove((*it));
			}
			else
			{
				delete(*it);
				*it = nullptr;
			}
			it = _particles.erase(it);
		}
	}
	// Recorremos generadores para añadir la lista de particulas generadas
	for (ParticleGenerator* g: _particles_generator)
	{
		if (g->timerGenerator(t) && g->maxNumEnts() && !g->getInput())
		{
			std::list<Entity*> listP = g->generateParticles();
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
	// Particulas de fuerzas de flote
	for (auto it = _bouyancy_particles.begin(); it != _bouyancy_particles.end();)
	{
		RigidSolid* p = static_cast<RigidSolid*>(*it);
		// Muere por tiempo 
		if (p->getTimeLeft() > 0)
		{
			p->integrate(t);
			float y = p->getVel().y;
			p->setVel({ 0,y,0 });
			p->clearAllForces();
			p->setRot(physx::PxQuat{1,0,0,0});
			
			++it;
		}
		else
		{
			_particle_force_registry->deleteParticleRegistry(p);
			delete(*it);
			*it = nullptr;
			it = _bouyancy_particles.erase(it);
		}
	}
	for (auto it = _bouyancy_force_generators.begin(); it != _bouyancy_force_generators.end();)
	{
		(*it)->updateTime(t);
		// En caso de que se pase la duración, se borra el generador de fuerzas
		if (!(*it)->checkTime())
		{
			_particle_force_registry->deleteForceGeneratorRegistry((*it));
			auto aux = (*it);
			it = _bouyancy_force_generators.erase(it);
			delete(aux);
		}
		else ++it;
	}
	_particle_force_registry->updateForces(t);
	
}

void ParticleSystem::generateSpringForce()
{
	Particle* p1 = new Particle({20.0,50.0,0.0}, {0.0,0.0,0.0}, {0.0,0.0,0.0}, {0.0,0.0,0.0}, 0.85, 0, 0, 60, Sphere, 1, { 255, 255,255,1 });
	Particle* p2 = new Particle({ -20,50,0 }, { 0,0,0,}, { 0,0,0 }, { 0,0,0 }, 0.85, 0, 0, 60, Sphere, 1, { 0, 0,0,1 });
	p2->setMass(2.0);
	p1->setMass(2.0);
	f1 = new SpringForceGenerator(1,20, p2);
	_particle_force_registry->addRegistry(f1, p1);
	f2 = new SpringForceGenerator(1,20,p1);
	_particle_force_registry->addRegistry(f2, p2);
	_force_generators.push_back(f1);
	_force_generators.push_back(f2);
	_particles.push_back(p1);
	_particles.push_back(p2);
	
}

void ParticleSystem::generateSpringForceRigidSolid(physx::PxPhysics* gPhysics, physx::PxScene* gScene)
{
	RigidSolid* p1 = new RigidSolid(Vector3(10, 10, 0), Dynamic, 2, gPhysics, gScene, 2, Sphere, 2, { 255, 255,255,1 });
	RigidSolid* p2 = new RigidSolid(Vector3(-10, 10, 0), Dynamic, 2, gPhysics, gScene, 2, Sphere, 2, { 0, 0,0,1 });
	f1 = new SpringForceGenerator(1, 20, p2);
	_particle_force_registry->addRegistry(f1, p1);
	f2 = new SpringForceGenerator(1, 20, p1);
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

void ParticleSystem::generateBuoyancyForce(physx::PxPhysics* gPhysics, physx::PxScene* gScene)
{
	// LÍQUIDO
	liquid = new Particle({ 0.0, -20.0, -30.0 }, { 0.0,0.0,0.0 }, { 0.0,0.0,0.0 }, { 0.0,0.0,0.0 },
		0.85, 0.0, 0.0, 60, Liquid);

	// RIGIDO 1
	RigidSolid* p = new RigidSolid({ 0.0, -10.0, -30.0 }, Dynamic, 500, gPhysics, gScene, 50, RectangleBox, 10, { 1.0,0.0,0.0,1 }, true);
	BouyancyForceGenerator* bGF = new BouyancyForceGenerator(p->getHeigth(), p->getVolumen(), 1000, liquid);
	// Lo añadimos al registro y le asociamos la fuerza correspondiente
	_particle_force_registry->addRegistry(bGF, p);
	addBouyancyForces(bGF, p);

	// RIGIDO 2
	RigidSolid* p2 = new RigidSolid({ -20.0, -10.0, -30.0 }, Dynamic, 150, gPhysics, gScene, 50, RectangleBox, 8, { 1.0,0.2,0.02,1 }, true);
	BouyancyForceGenerator* bGF2 = new BouyancyForceGenerator(p2->getHeigth(), p2->getVolumen(), 1000, liquid);
	// Lo añadimos al registro y le asociamos la fuerza correspondiente
	_particle_force_registry->addRegistry(bGF2, p2);
	addBouyancyForces(bGF2, p2);

	// RIGIDO 3
	RigidSolid* p3 = new RigidSolid({ 20.0, -10.0, -30.0 }, Dynamic, 280, gPhysics, gScene, 50, RectangleBox, 6, { 1.0, 0.1,0.1,1 }, true);
	BouyancyForceGenerator* bGF3 = new BouyancyForceGenerator(p3->getHeigth(), p3->getVolumen(), 1000, liquid);
	// Lo añadimos al registro y le asociamos la fuerza correspondiente
	_particle_force_registry->addRegistry(bGF3, p3);
	addBouyancyForces(bGF3, p3);

}

void ParticleSystem::deleteLists()
{
	// Borramos particulas/solidos rigidos
	for (auto it = _particles.begin(); it != _particles.end();)
	{
		_particle_force_registry->deleteParticleRegistry((*it));
		delete(*it);
		*it = nullptr;
		it = _particles.erase(it);
	}
	// Borramos generadores de particulas
	for (auto it = _particles_generator.begin(); it != _particles_generator.end(); )
	{
		delete(*it);
		*it = nullptr;
		it = _particles_generator.erase(it);
	}
	// Borramos generadores de fuerza
	for (auto it = _force_generators.begin(); it != _force_generators.end(); )
	{
		_particle_force_registry->deleteForceGeneratorRegistry((*it));
		delete(*it);
		*it = nullptr;
		it = _force_generators.erase(it);
	}
	// Borramos entidades flotantes
	for (auto it = _bouyancy_particles.begin(); it != _bouyancy_particles.end();)
	{
		_particle_force_registry->deleteParticleRegistry((*it));
		delete(*it);
		*it = nullptr;
		it = _bouyancy_particles.erase(it);
	}
	// Borramos fuerzas de flotación
	for (auto it = _bouyancy_force_generators.begin(); it != _bouyancy_force_generators.end(); )
	{
		_particle_force_registry->deleteForceGeneratorRegistry((*it));
		delete(*it);
		*it = nullptr;
		it = _bouyancy_force_generators.erase(it);
	}
	// Borramos agua
	if (liquid != nullptr)
	{
		delete liquid;
		liquid = nullptr;
	}
}

void ParticleSystem::deleteParticles()
{
	deleteLists();
	setActive(false);
}

void ParticleSystem::checkColissionsWithBullets(int& score, physx::PxActor* actor1, physx::PxActor* actor2, std::list<Entity*>& _bulletParticlesList)
{
	// Recorremos la lista de balas para ver sus colisiones
	for (auto it = _bulletParticlesList.begin(); it != _bulletParticlesList.end();++it )
	{
		RigidSolid* bala = static_cast<RigidSolid*>((*it));

		// Colisiones con los objetivos del juego
		for (auto it2 = _particles.begin(); it2 != _particles.end();)
		{
			RigidSolid* target = static_cast<RigidSolid*>((*it2));
			if (bala->getActor() != target->getActor() && (bala->getActor() == actor1 || bala->getActor() == actor2) && (target->getActor() == actor1 || target->getActor() == actor2))
			{
				// Actualizamos la puntuacion
				if (bala->getPos().p.y < 10 && bala->getPos().p.y > -15) score += 5;
				else if (bala->getPos().p.y > 10) updateScore(score);

				// Borramos bala
				_particles.remove(bala);
				delete(bala);
				bala = nullptr;
				it = _bulletParticlesList.erase(it);

				//Borramos la particula
				_particle_force_registry->deleteParticleRegistry(target);
				delete(target);
				target = nullptr;
				it2 = _particles.erase(it2);

				return;
			}
			else
			{
				++it2;
			}
		}
		// Colisiones con los obstaculos flotantes
		for (auto it3 = _bouyancy_particles.begin(); it3 != _bouyancy_particles.end();)
		{
			RigidSolid* target = static_cast<RigidSolid*>((*it3));
			if (bala->getActor() != target->getActor() && (bala->getActor() == actor1 || bala->getActor() == actor2) && (target->getActor() == actor1 || target->getActor() == actor2))
			{
				// Restamos puntos
				if(score > 0) --score;
				
				// Borramos bala
				_particles.remove(bala);
				delete(bala);
				bala = nullptr;
				it = _bulletParticlesList.erase(it);

				// Generamos explosion con firework
				addExplosion({0, -10, -100});
				Fireworks({ 0, -10, -100 }, {0,-7,-100}, 2, 0.3, 1);
				return;
			}
			else
			{
				++it3;
			}
		}

	}
}

void ParticleSystem::createFireWork(Vector3 mPos, Vector3 pos, int g, double t, int numModel)
{
	gausFireworkGenerator = new GaussianParticleGenerator("GaussianFireworkGenerator", mPos, { 1,50,1 }, { 1,10,1 }, 3, true, 0.1f);
	auto model = models::modelsFirework[numModel];
	Firework* p = new Firework(g, pos, Vector3(0, 0, 0),
		{ 0,0,0 }, { 0,0,0 }, model.damping, 1.0, 9.8, t, Sphere, model.scale, model.color);
	gausFireworkGenerator->setParticle(p, false);
	p->eraseVisualModel();
	addGenerator(gausFireworkGenerator);
}

ParticleSystem::~ParticleSystem()
{
	deleteLists();
}
