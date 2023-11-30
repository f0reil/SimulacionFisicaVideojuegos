#include "AnchoredSpringFG.h"

AnchoredSpringFG::AnchoredSpringFG(double k, double resting_lenght, const Vector3& anchor_pos) :
	SpringForceGenerator(k, resting_lenght, nullptr)
{
	_other = new Particle(anchor_pos, { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0, 0, 0, 1e6, Box);
}

AnchoredSpringFG::~AnchoredSpringFG()
{
	delete _other;
}
