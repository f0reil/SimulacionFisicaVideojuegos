#include "SpringForceGenerator.h"

SpringForceGenerator::SpringForceGenerator(double k, double resting_lenght, Particle* other) 
	: ForceGenerator(), _k(k), _resting_lenght(resting_lenght), _other(other)
{
}

void SpringForceGenerator::updateForce(Particle* particle, double t)
{
	Vector3 relative_pos_vector = _other->getPos().p - particle->getPos().p;
	Vector3 force;

	const float lenght = relative_pos_vector.normalize();
	const float delta_x = lenght - _resting_lenght;

	force = relative_pos_vector * delta_x * _k;

	particle->addForce(force);
}
