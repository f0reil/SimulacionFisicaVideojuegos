#pragma once
#include "ParticleDragGenerator.h"

class WhirlwindForceGenerator: public ParticleDragGenerator
{
public:
	WhirlwindForceGenerator(const int k, Vector3 wVel, Vector3 pOrigin, Vector3 limits, const float k1, const float k2)
		: k(k),ParticleDragGenerator(wVel, pOrigin, limits, k1, k2) {};
	~WhirlwindForceGenerator() {};

	virtual void updateForce(Entity* p, double t);

protected:
	int k = 0;
};

