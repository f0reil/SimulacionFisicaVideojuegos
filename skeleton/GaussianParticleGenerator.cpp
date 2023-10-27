#include "GaussianParticleGenerator.h"

std::list<Particle*> GaussianParticleGenerator::generateParticles()
{
    Particle* p = _model->clone();
    p->setPos(_mean_pos);
    p->setVel(Vector3((*velX)(gen), (*velY)(gen), (*velZ)(gen)));
    std::list<Particle*> a;
    a.push_back(p);
    return a;
}
