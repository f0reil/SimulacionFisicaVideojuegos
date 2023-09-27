#include "Particle.h"

Particle::Particle(Vector3 pos, Vector3 vel)
{
	this->velo = vel;
	// Creamos la particula con forma de esfera con un color y una posición
	pose = physx::PxTransform(pos);
	physx::PxSphereGeometry sphere(5);
	physx::PxShape* shape = CreateShape(sphere);
	const Vector4 color = {255, 250, 0, 1};
	renderItem = new RenderItem(shape, &pose, color);
}

void Particle::integrate(double t)
{
	pose.p += velo * t;
}

Particle::~Particle()
{
	if (renderItem != nullptr)
	{
		renderItem->release();
	}
}