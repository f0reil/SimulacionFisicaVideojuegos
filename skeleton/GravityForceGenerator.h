#pragma once
#include "ForceGenerator.h"
#include "core.hpp"

class GravityForceGenerator: public ForceGenerator
{
public:
	GravityForceGenerator(const Vector3& g);

	virtual void updateForce(Entity* particle, double t);

	inline void setGravity(Vector3 g) { _gravity = g; };
protected:
	Vector3 _gravity;
};

