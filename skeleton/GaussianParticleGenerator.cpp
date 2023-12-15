#include "GaussianParticleGenerator.h"

std::list<Entity*> GaussianParticleGenerator::generateParticles()
{
    Entity* p = _model->clone();
    p->setPos(_mean_pos);
    p->setVel(Vector3((*velX)(gen), (*velY)(gen), (*velZ)(gen)));
    std::list<Entity*> a;
    a.push_back(p);
    ++contEnts;
    return a;
}
