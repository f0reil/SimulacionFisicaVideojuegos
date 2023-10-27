#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include <list>

class ParticleGenerator;

class Particle
{
public:
	Particle(Vector3 pos, Vector3 velR, Vector3 velS, Vector3 ac,
		double damping, double mass, double gravity, double timeLife,
		int scale = 5, Vector4 color = { 255, 250, 0, 1 }, bool proyectil = false);
	virtual ~Particle();
	virtual void integrate(double t);
	virtual Particle* clone() const; 
	bool getGeneratesOnDeath() { return generatesOnDeath; }
	ParticleGenerator* getGenerator() { return _particle_generator; }
	double getTimeLeft() { return remaining_time; }
	bool insideLimits() { return pose.p.x > posI.x + 10000 || pose.p.y > posI.y + 10000 || pose.p.z > posI.z + 10000
						|| pose.p.x < posI.x -10000 || pose.p.y < posI.y -10000|| pose.p.z < posI.z -10000; };
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



};

