#include "Particle.h"

Particle::Particle(Vector3 pos, Vector3 dir, Vector3 velR, Vector3 velS, Vector3 ac,
	double damping, double mass, double gravity)
{
	this->velo = dir * velS.x;
	this->a = ac + Vector3(0, -gravity, 0);
	this->d = damping;

	// Calculamos masa simulada------------------------
	m = mass * pow((velR.x / velS.x), 2);

	// Calculamos gravedad simulada--------------------
	gSim = -gravity * pow(velS.x / velR.x, 2);

	// Creamos la particula con forma de esfera con un color y una posición-----------------
	pose = physx::PxTransform(pos);
	physx::PxSphereGeometry sphere(5);
	physx::PxShape* shape = CreateShape(sphere);
	const Vector4 color = {255, 250, 0, 1};
	renderItem = new RenderItem(shape, &pose, color);
}

void Particle::integrate(double t)
{
	pose.p += velo * t;

	velo += a * t;

	velo *= pow(d, t);
}

Particle::~Particle()
{
	if (renderItem != nullptr)
	{
		renderItem->release();
	}
}