#include "FireworkGenerator.h"
#include "Firework.h"

std::list<Particle*> FireworkGenerator::generateParticles()
{
    std::list<Particle*> f;
    for (int i = 0; i < num_particles; ++i)
    {
        Particle* p = _model->clone();
        p->setPos(_mean_pos);
        p->setVel(Vector3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50));
        f.push_back(p);
    }
    setDestroy(true);
    return f;
}
