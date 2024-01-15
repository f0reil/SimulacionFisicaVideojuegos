#include "Particle.h"
#include "ParticleGenerator.h"
#include <iostream>
Particle::Particle(Vector3 pos, Vector3 velR, Vector3 velS, Vector3 ac,
	double damping, double mass, double gravity, double timeLife, Geometry forma, float scale, Vector4 color, 
	bool proyectil, bool firework)
	: Entity (timeLife, forma, scale, color, proyectil, firework)
{
	// PROYECTIL-------------------
	if (proyectil)
	{
	this->velo = velS;
	//this->a = ac + Vector3(0, gravity, 0);

		// Calculamos masa simulada------------------------
		m = mass * pow((velR.z / velS.z), 2);

		// Calculamos gravedad simulada--------------------
		gSim = -gravity * pow(velS.x / velR.x, 2);
	}
	else
	// PARTICULA
	{
		this->velo = velS;
		//this->a = ac;
		m = mass;
	}
	
	this->d = damping;

	// Creamos la particula con forma de esfera con un color y una posición-----------------
	pose = physx::PxTransform(pos);

	renderItem = new RenderItem(entShape, &pose, colorP);

	timeI = remaining_time = timeLife;

	posI = pose.p;
}

void Particle::integrate(double t)
{
	Vector3 resulting_accel = force * (1/m);
	velo += resulting_accel * t; 
	velo *= powf(d, t); 
	pose.p += velo * t;
	Entity::integrate(t);
	
}

Entity* Particle::clone() const
{
	Entity* p = new Particle(pose.p, { 0,0,0 }, velo, {0,0,0}, d, m, gReal, timeI, Sphere, scaleP, colorP);
	return p;
}

std::list<Entity*> Particle::getGeneratedParticles()
{
	return _particle_generator->generateParticles();
}

Particle::~Particle()
{
	
}