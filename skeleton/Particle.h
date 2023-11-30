#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include <list>
#include <iostream>
class ParticleGenerator;

enum Geometry { Sphere, Box, Liquid};

class Particle
{
public:
	Particle(Vector3 pos, Vector3 velR, Vector3 velS, Vector3 ac,
		double damping, double mass, double gravity, double timeLife, Geometry forma = Sphere,
		int scale = 5, Vector4 color = { 255, 250, 0, 1 }, bool proyectil = false);
	virtual ~Particle();
	virtual void integrate(double t);
	virtual Particle* clone() const; 
	bool getGeneratesOnDeath() { return generatesOnDeath; }
	ParticleGenerator* getGenerator() { return _particle_generator; }
	double getTimeLeft() { return remaining_time; }
	bool insideLimits( Vector3 limit) { return pose.p.x > posI.x + limit.x || pose.p.y > posI.y + limit.y || pose.p.z > posI.z + limit.z
						|| pose.p.x < posI.x -limit.x || pose.p.y < posI.y -limit.y|| pose.p.z < posI.z -limit.z; };
	bool insideLimits(Vector3 limit, Vector3 origin) {
		return pose.p.x > origin.x + limit.x || pose.p.y > origin.y + limit.y || pose.p.z > origin.z + limit.z
			|| pose.p.x < origin.x - limit.x || pose.p.y < origin.y - limit.y || pose.p.z < origin.z - limit.z;
	};
	inline physx::PxTransform getPos() { return pose; }
	inline Vector3 getVel() { return velo; } 
	inline void setPos(Vector3 p) { pose.p = p; }
	inline void setVel(Vector3 v) { velo = v; }
	virtual void death() {};
	std::list<Particle*> getGeneratedParticles();
	inline void eraseVisualModel()
	{
		if (renderItem != nullptr)
		{
			renderItem->release();
			renderItem = nullptr;
		}
	}

	inline void clearForce() { force = force * 0; };
	inline void addForce(const Vector3& f) { force += f; };

	inline double getMass() { return m; };
	inline double getInvMass() { return 1 / m; };

	inline void setMass(double mass) { m = mass; };

	inline int getVolumen() 
	{
		int volumen;
		switch (formaP)
		{
		case Box:
			volumen = pow(scaleP, 3);
			break;
		case Sphere:
			volumen = 4 / 3 * std::atan(1) * 4 * scaleP * scaleP * scaleP;
			break;
		default:
			break;
		}
		return volumen;
	};
protected:
	Vector3 velo;
	Vector3 a;
	Vector3 posI;
	Vector4 colorP;
	Vector3 posP, dirP, velRP, velS, acP;
	physx::PxTransform pose;
	
	RenderItem* renderItem;
	ParticleGenerator* _particle_generator = nullptr;
	
	double d, m, gSim, gReal;
	double remaining_time, timeI;
	int scaleP;
	bool generatesOnDeath = false;

	Vector3 force = {0,0,0};

	Geometry formaP;
};

