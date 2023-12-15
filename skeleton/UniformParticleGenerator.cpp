#include "UniformParticleGenerator.h"
#include <iostream>
#include "ecs.h"

std::list<Entity*> UniformParticleGenerator::generateParticles()
{
    Entity* p = _model->clone();
    p->setPos(_mean_pos + Vector3((*posX)(gen), (*posY)(gen), (*posZ)(gen)));
    p->setVel(_mean_vel + Vector3((*velX)(gen), (*velY)(gen), (*velZ)(gen)));
    std::list<Entity*> a;
    a.push_back(p);
    ++contEnts;
    return a;
}
