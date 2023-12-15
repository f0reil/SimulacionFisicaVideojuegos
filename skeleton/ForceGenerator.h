#pragma once
#include "Particle.h"

class ForceGenerator
{
public:
	ForceGenerator() {};
	virtual void updateForce(Entity* particle, double t) = 0;

	std::string _name;
	double _t = 0.0; // if negative ---> eternal
	double _duration = -1e10;

	inline void updateTime(double t) 
	{ 
		_t += t; 
	};
	inline bool checkTime()
	{
		return _t < _duration || _duration < 0.0;
	}
	inline void setDuration(double d) { _duration = d; };
	virtual ~ForceGenerator() {};
};

