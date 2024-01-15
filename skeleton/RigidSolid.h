#pragma once
#include "Entity.h"

enum Type {Static, Dynamic};

class RigidSolid : public Entity
{
public:
	RigidSolid(Vector3 pos, Type type,double mass, physx::PxPhysics* gPhysics, physx::PxScene* gScene, 
		double timeLife, Geometry forma, int scale, Vector4 color, bool isStatic = false);
	~RigidSolid() {};

	virtual void integrate(double t);
	virtual Entity* clone() const;
	virtual inline void clearForce() { };
	virtual inline void addForce(const Vector3& f) {
		if (type == Dynamic)
		{
			_solidDynamic->addForce(f);
		}
	};

	virtual inline void setMass(double mass) 
	{ 
		m = mass;
		_solidDynamic->setMass(m);
	};

	virtual inline void setVel(Vector3 v) {
		if (type == Dynamic)
		{
			_solidDynamic->setLinearVelocity(v);
		}
	}; 
	virtual inline void setPos(Vector3 p) {
		if (type == Dynamic)
		{
			physx::PxTransform tr = _solidDynamic->getGlobalPose();
			tr.p = p;
			_solidDynamic->setGlobalPose(tr);
		}
		else
		{
			physx::PxTransform tr = _solidStatic->getGlobalPose();
			tr.p = p;
			_solidStatic->setGlobalPose(tr);
		}
		posI = p;
	}
	virtual inline Vector3 getVel() {
		if (type == Dynamic)
		{
			return _solidDynamic->getLinearVelocity();
		}
		else
		{
			return { 0,0,0 };
		}
	}
	virtual inline physx::PxTransform getPos() 
	{
		if (type == Dynamic)
		{
			return _solidDynamic->getGlobalPose();
		}
		else
		{
			return _solidStatic->getGlobalPose();
		}
	}

	virtual bool insideLimits(Vector3 limit) {
		if (type == Dynamic)
		{
			Vector3& pos = _solidDynamic->getGlobalPose().p;
			return pos.x > posI.x + limit.x || pos.y > posI.y + limit.y || pos.z > posI.z + limit.z
			|| pos.x < posI.x - limit.x || pos.y < posI.y - limit.y || pos.z < posI.z - limit.z;
		}
		return true;
	};
	virtual bool insideLimits(Vector3 limit, Vector3 origin) {
		if (type == Dynamic)
		{
			Vector3& pos = _solidDynamic->getGlobalPose().p;
			return pos.x > origin.x + limit.x || pos.y > origin.y + limit.y || pos.z > origin.z + limit.z
				|| pos.x < origin.x - limit.x || pos.y < origin.y - limit.y || pos.z < origin.z - limit.z;
		}
	};

	virtual std::list<Entity*> getGeneratedParticles();

	inline physx::PxRigidActor* getActor() { return _actor; };

	void inline clearAllForces()
	{
		if (type == Dynamic)
		{
			_solidDynamic->clearForce(physx::PxForceMode::eFORCE);
			_solidDynamic->clearForce(physx::PxForceMode::eACCELERATION);
			_solidDynamic->clearForce(physx::PxForceMode::eIMPULSE);
			_solidDynamic->clearForce(physx::PxForceMode::eVELOCITY_CHANGE);
		}
	}

	virtual inline void setRot(physx::PxQuat q) {
		if (type == Dynamic)
		{
			auto tr = _solidDynamic->getGlobalPose();
			tr.q = q;
			_solidDynamic->setGlobalPose(tr);
		}
	}

protected:
	physx::PxPhysics* gPhysics;
	physx::PxScene* gScene;

	physx::PxRigidStatic* _solidStatic = nullptr;
	physx::PxRigidDynamic* _solidDynamic = nullptr;

	physx::PxRigidActor* _actor = nullptr;

	Type type;
};

