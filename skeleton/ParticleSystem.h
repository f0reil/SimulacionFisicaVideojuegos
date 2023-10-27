#pragma once
#include "Particle.h"
#include "ParticleGenerator.h"
#include <list>
#include <unordered_map>
class ParticleSystem
{
public:
	ParticleSystem() {};
	~ParticleSystem();

	void update(double t);
	inline ParticleGenerator* getParticleGenerator(const std::string& n)
	{
		auto it = _pGenerator_map.find(n);
		if (it == _pGenerator_map.end()) throw std::invalid_argument("generator does not exist");
		else return (*it).second;
	};
	inline void addGenerator(ParticleGenerator* g)
	{ 
		_particles_generator.push_back(g);
		_pGenerator_map[g->getGeneratorName()] = g;
	};
	inline void shootFirework()
	{
		_particles.splice(_particles.end(), getParticleGenerator("GaussianFireworkGenerator")->generateParticles());
	}
private:
	std::list<Particle*> _particles;
	std::list<ParticleGenerator*> _particles_generator;
	std::unordered_map<std::string, ParticleGenerator*> _pGenerator_map;

};

