#include "Particle.h"
#include "ParticleGenerator.h"
#include <iostream>
Particle::Particle(Vector3 pos, Vector3 velR, Vector3 velS, Vector3 ac,
	double damping, double mass, double gravity, double timeLife, int scale, Vector4 color, bool proyectil)
{
	scaleP = scale;
	colorP = color;

	// PROYECTIL-------------------
	if (proyectil)
	{
	this->velo = velS;
	//this->a = ac + Vector3(0, gravity, 0);

		// Calculamos masa simulada------------------------
		m = mass * pow((velR.x / velS.x), 2);

		// Calculamos gravedad simulada--------------------
		//gSim = -gravity * pow(velS.x / velR.x, 2);
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
	physx::PxSphereGeometry sphere(scaleP);
	physx::PxShape* shape = CreateShape(sphere);
	renderItem = new RenderItem(shape, &pose, colorP);

	timeI = remaining_time = timeLife;

	posI = pose.p;
}

void Particle::integrate(double t)
{
	Vector3 resulting_accel = force * (1/m);
	velo += resulting_accel * t; 
	velo *= powf(d, t); 
	pose.p += velo * t;
	remaining_time -= t;
	clearForce();
}

Particle* Particle::clone() const
{
	Particle* p = new Particle(pose.p, { 0,0,0 }, velo, {0,0,0}, d, m, gReal, timeI, scaleP, colorP);
	return p;
}

std::list<Particle*> Particle::getGeneratedParticles()
{
	return _particle_generator->generateParticles();
}

Particle::~Particle()
{
	if (renderItem != nullptr)
	{
		renderItem->release();
	}
	if (_particle_generator != nullptr)
	{
		delete(_particle_generator);
	}
}