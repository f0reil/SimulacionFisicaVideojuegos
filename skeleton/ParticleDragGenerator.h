#pragma once
#include "ForceGenerator.h"
#include "core.hpp"

class ParticleDragGenerator: public ForceGenerator
{
public:
	ParticleDragGenerator();
	ParticleDragGenerator(Vector3 wVel, Vector3 pOrigin, Vector3 limits, const float k1, const float k2);
	~ParticleDragGenerator() {};

	virtual void updateForce(Particle* p, double t);
	inline void setDrag(float k1, float k2) { _k1 = k1; _k2 = k2; };
	inline float getK1() { return (_k1); };
	inline float getK2() { return (_k2); };
protected:
	float _k1 = 0; // Coef velocity
	float _k2 = 0; // Coef square velocity
	Vector3 windVel = { 0,0,0 };
	Vector3 limit;
	Vector3 origins;
};

