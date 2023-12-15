#include "Firework.h"
#include <iostream>
#include "ecs.h"
void Firework::integrate(double t)
{
	Entity::integrate(t);
}
Entity* Firework::clone() const
{
	Firework* f = new Firework(g, pose.p, { 0,0,0 }, velo, {0,0,0}, d, m, gReal, timeI, Sphere, scaleP, colorP);
	return f;
}
void Firework::death()
{
	if (g != 0)
	{
		generatesOnDeath = true;
		// Creamos nuevo generador
		_particle_generator = new FireworkGenerator("Firework" + std::to_string(g + velS.y), { 30,30,30 }, posP, 5, g);
		_particle_generator->setPos(pose.p);
		if (scaleP > 1) scaleP=scaleP - 1;
		// Seteamos modelo del generador
		auto model = models::modelsFirework[rand() % models::modelsFirework.size()];
		Firework* f = new Firework(g - 1, pose.p, { 0,0,0 }, velo, { 0,0,0 }, model.damping, m, gReal, (rand() % 100) / 100.0, Sphere, model.scale, model.color);
		f->eraseVisualModel();
		_particle_generator->setParticle(f);
	}

}

