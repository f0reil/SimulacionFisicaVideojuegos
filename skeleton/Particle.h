#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"

class Particle
{
public:
	Particle(Vector3 pos, Vector3 dir, Vector3 velR, Vector3 velS, Vector3 ac, 
				double damping, double mass, double gravity);
	~Particle();

	void integrate(double t);
private:
	Vector3 velo;
	Vector3 a;
	physx::PxTransform pose;
	RenderItem* renderItem;
	double d, m, gSim;

	//plantear la gravedad como un vector 

};

