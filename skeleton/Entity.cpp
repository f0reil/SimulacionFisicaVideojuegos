#include "Entity.h"

Entity::Entity(double timeLife, Geometry forma, float scale, Vector4 color, bool proyect, bool isHold)
{
	hold = isHold;
	formaP = forma;
	scaleP = scale;
	colorP = color;
	proyectil = proyect;
	
	switch (forma)
	{
	case Sphere:
		entShape = CreateShape(physx::PxSphereGeometry(scaleP));
		break;
	case Box:
		entShape = CreateShape(physx::PxBoxGeometry(Vector3(scaleP, scaleP, scaleP)/2));
		break;
	case Liquid:
		entShape = CreateShape(physx::PxBoxGeometry(Vector3(60, 4.5, 10)));
		colorP = Vector4(0.8, 0.0, 1.0, 1);
		break;
	case RectangleBox:
		if (hold)
		{
			physx::PxMaterial* m;
			m = PxGetPhysics().createMaterial(100000, 100000, -1);
			entShape = CreateShape(physx::PxBoxGeometry(Vector3(scaleP / 2, scaleP, scaleP / 2) ), m);
		}
		else
		{
			entShape = CreateShape(physx::PxBoxGeometry(Vector3(scaleP / 2, scaleP, scaleP / 2)));
		}
		break;
	case RectangleBox2:
		entShape = CreateShape(physx::PxBoxGeometry(Vector3(scaleP, scaleP/2, scaleP / 2)));
		break;
	case Grass:
		entShape = CreateShape(physx::PxBoxGeometry(Vector3(150, 4.5, 60)));
		colorP = Vector4(0, 1.0, 0.7, 1);
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
