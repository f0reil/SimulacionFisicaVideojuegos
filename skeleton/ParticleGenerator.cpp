#include "ParticleGenerator.h"

void ParticleGenerator::setParticle(Entity* p, bool modify_pos_vel)
{
	if(_model != nullptr) delete _model;
	_model = p->clone();
	if (modify_pos_vel) {
		_mean_pos = p->getPos().p;
		_mean_vel = p->getVel();
	}
	_model->setPos({ -1000.0f, -1000.0f, -1000.0f });
	
}
