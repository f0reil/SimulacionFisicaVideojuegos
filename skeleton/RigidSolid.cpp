#include "RigidSolid.h"
#include "ParticleGenerator.h"

RigidSolid::RigidSolid(Vector3 pos, Type type, double mass, physx::PxPhysics* gPhysics, 
	physx::PxScene* gScene,double timeLife, Geometry forma, int scale, Vector4 color, bool isStatic)
	: Entity(timeLife, forma, scale, color, false, isStatic), gPhysics(gPhysics), gScene(gScene)
{
	m = mass;
	posI = pos;
	this->type = type;
	switch (type)
	{
	case Static:
		_actor = _solidStatic = gPhysics->createRigidStatic(physx::PxTransform(pos));
		_solidStatic->attachShape(*entShape);
		gScene->addActor(*_solidStatic);
		renderItem = new RenderItem(entShape, _solidStatic, colorP);
		break;
	case Dynamic:
		_actor = _solidDynamic = gPhysics->createRigidDynamic(physx::PxTransform(pos));
		_solidDynamic->attachShape(*entShape);
		physx::PxRigidBodyExt::updateMassAndInertia(*_solidDynamic, physx::PxReal(m / getVolumen()));
		gScene->addActor(*_solidDynamic);
		renderItem = new RenderItem(entShape, _solidDynamic, colorP);
		break;
	}

}
void RigidSolid::integrate(double t)
{
	Entity::integrate(t);
}

Entity* RigidSolid::clone() const
{
	if (type == Dynamic)
	{
		return new RigidSolid(_solidDynamic->getGlobalPose().p, type, m, gPhysics, gScene, timeI, formaP, scaleP, colorP);
	}
	else
	{
		return new RigidSolid(_solidStatic->getGlobalPose().p, type, m, gPhysics, gScene, timeI, formaP, scaleP, colorP);
	}
}

std::list<Entity*> RigidSolid::getGeneratedParticles()
{
	return _particle_generator->generateParticles();
}
