#pragma once
#include "Particle.h"
#include "FireworkGenerator.h"

class Firework: public Particle
{
public:
	Firework(int generation, Vector3 pos, Vector3 velR, Vector3 velS, Vector3 ac,
		double damping, double mass, double gravity, double timeLife,Geometry forma, int scale, Vector4 color)
		: Particle(pos, velR, velS, ac, damping, mass, gravity, timeLife, forma, scale, color, false) 
	{
			g = generation;
	}
	virtual ~Firework() {};

	virtual void integrate(double t);

	virtual Entity* clone() const;
	virtual void death();

private: 
	int g;
};

