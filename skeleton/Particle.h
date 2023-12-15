#pragma once
#include "Entity.h"


class Particle: public Entity
{
public:
	Particle(Vector3 pos, Vector3 velR, Vector3 velS, Vector3 ac,
		double damping, double mass, double gravity, double timeLife, Geometry forma = Sphere,
		int scale = 5, Vector4 color = { 255, 250, 0, 1 }, bool proyectil = false);
	virtual ~Particle();
	virtual void integrate(double t);
	virtual Entity* clone() const;
	virtual inline void clearForce() { force = force * 0; };
	virtual inline void addForce(const Vector3& f) { force += f; };

	virtual inline void setMass(double mass) { m = mass; };

	virtual inline void setVel(Vector3 v) { velo = v; };
	virtual inline void setPos(Vector3 p) { pose.p = p; }
	virtual inline Vector3 getVel() { return velo; }
	virtual inline physx::PxTransform getPos() { return pose; }

	virtual bool insideLimits(Vector3 limit) {
		return pose.p.x > posI.x + limit.x || pose.p.y > posI.y + limit.y || pose.p.z > posI.z + limit.z
			|| pose.p.x < posI.x - limit.x || pose.p.y < posI.y - limit.y || pose.p.z < posI.z - limit.z;
	};
	virtual bool insideLimits(Vector3 limit, Vector3 origin) {
		return pose.p.x > origin.x + limit.x || pose.p.y > origin.y + limit.y || pose.p.z > origin.z + limit.z
			|| pose.p.x < origin.x - limit.x || pose.p.y < origin.y - limit.y || pose.p.z < origin.z - limit.z;
	};

	virtual std::list<Entity*> getGeneratedParticles();

protected:
	Vector3 velo;
	Vector3 a;
	Vector3 posP, dirP, velRP, velS, acP;
	physx::PxTransform pose;
	
	
	double d, gSim, gReal;

	Vector3 force = {0,0,0};

};

