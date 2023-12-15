#include "Entity.h"

Entity::Entity(double timeLife, Geometry forma, int scale, Vector4 color)
{
	formaP = forma;
	scaleP = scale;
	colorP = color;

	
	switch (forma)
	{
	case Sphere:
		entShape = CreateShape(physx::PxSphereGeometry(scaleP));
		break;
	case Box:
		entShape = CreateShape(physx::PxBoxGeometry(Vector3(scaleP, scaleP, scaleP)/2));
		break;
	case Liquid:
		entShape = CreateShape(physx::PxBoxGeometry(Vector3(20, 1.5, 10)));
		colorP = Vector4(0, 255, 255, 1);
		break;
	}

	timeI = remaining_time = timeLife;
}

Entity::~Entity()
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

void Entity::integrate(double t)
{
	remaining_time -= t;
	clearForce();
}
