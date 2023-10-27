#pragma once
#include "ParticleGenerator.h"
#include <chrono>

class Firework;

class FireworkGenerator: public ParticleGenerator
{
public:
	FireworkGenerator(std::string name, Vector3 mPos, Vector3 mVe, int n, int g, bool input = false) 
		: ParticleGenerator (name, mPos, mVe, n, input), generation(g)
		{}
	~FireworkGenerator() {}

	virtual std::list<Particle*> generateParticles();

	inline bool getDestroy() { return destroy; };
	inline void setDestroy(bool d) { destroy = d; };
private:
	bool destroy = false; 
	int generation;

};

